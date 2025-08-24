#include "canbuscontroller.h"
#include <QDebug>
#include <QRandomGenerator>

#ifdef HAVE_QT_SERIALBUS
#include <QCanBus>
#endif

CanBusController::CanBusController(QObject *parent)
    : QObject(parent)
#ifdef HAVE_QT_SERIALBUS
    , m_canDevice(nullptr)
#endif
    , m_simulationTimer(new QTimer(this))
    , m_connected(false)
    , m_status("Disconnected")
    , m_speed(0)
    , m_rpm(800)
    , m_fuelLevel(85)
    , m_engineTemp(90)
    , m_leftTurnSignal(false)
    , m_rightTurnSignal(false)
    , m_headlights(false)
{
    connect(m_simulationTimer, &QTimer::timeout, this, &CanBusController::simulateVehicleData);
    setupSimulatedData();
}

CanBusController::~CanBusController()
{
#ifdef HAVE_QT_SERIALBUS
    if (m_canDevice) {
        m_canDevice->disconnectDevice();
        delete m_canDevice;
    }
#endif
}

bool CanBusController::connected() const
{
    return m_connected;
}

QString CanBusController::status() const
{
    return m_status;
}

void CanBusController::connectToSimulator()
{
    connectToBus("vcan0");
}

void CanBusController::connectToBus(const QString &interface)
{
    if (m_connected) {
        return;
    }

#ifdef HAVE_QT_SERIALBUS
    // Try to connect to specified interface (e.g., vcan0 for virtual CAN)
    qDebug() << "Attempting to connect to CAN interface:" << interface;
    
    // Create device using socketcan plugin with specified interface
    m_canDevice = QCanBus::instance()->createDevice(QStringLiteral("socketcan"), interface);
    
    if (!m_canDevice) {
        qDebug() << "Failed to create CAN device for interface:" << interface;
        // Check available devices
        QString errorString;
        auto availableDevices = QCanBus::instance()->availableDevices(QStringLiteral("socketcan"), &errorString);
        qDebug() << "Available devices:" << availableDevices.size();
        for (const auto &device : availableDevices) {
            qDebug() << "  -" << device.name() << device.description();
        }
    }

    if (m_canDevice) {
        connect(m_canDevice, &QCanBusDevice::framesReceived, this, &CanBusController::handleFramesReceived);
        connect(m_canDevice, &QCanBusDevice::errorOccurred, this, &CanBusController::handleErrorOccurred);
        connect(m_canDevice, &QCanBusDevice::stateChanged, this, &CanBusController::handleStateChanged);

        if (m_canDevice->connectDevice()) {
            m_status = "Connecting to CAN Bus...";
            emit statusChanged(m_status);
            return;
        } else {
            delete m_canDevice;
            m_canDevice = nullptr;
        }
    }
#endif

    // Start simulation mode (fallback or when SerialBus not available)
    m_status = "Simulation Mode Active";
    m_connected = true;
    m_simulationTimer->start(100); // 10Hz update rate
    emit connectedChanged(m_connected);
    emit statusChanged(m_status);
}

void CanBusController::disconnectFromSimulator()
{
    if (!m_connected) {
        return;
    }

    m_simulationTimer->stop();
    
#ifdef HAVE_QT_SERIALBUS
    if (m_canDevice) {
        m_canDevice->disconnectDevice();
        delete m_canDevice;
        m_canDevice = nullptr;
    }
#endif

    m_connected = false;
    m_status = "Disconnected";
    emit connectedChanged(m_connected);
    emit statusChanged(m_status);
}

void CanBusController::sendFrame(quint32 frameId, const QByteArray &data)
{
#ifdef HAVE_QT_SERIALBUS
    if (!m_canDevice || !m_connected) {
        return;
    }

    QCanBusFrame frame(frameId, data);
    m_canDevice->writeFrame(frame);
#else
    Q_UNUSED(frameId)
    Q_UNUSED(data)
#endif
}

#ifdef HAVE_QT_SERIALBUS
void CanBusController::handleFramesReceived()
{
    if (!m_canDevice) {
        return;
    }

    while (m_canDevice->framesAvailable()) {
        const QCanBusFrame frame = m_canDevice->readFrame();
        if (frame.isValid()) {
            emit frameReceived(frame.frameId(), frame.payload());
        }
    }
}

void CanBusController::handleErrorOccurred(QCanBusDevice::CanBusError error)
{
    if (m_canDevice) {
        QString errorString = m_canDevice->errorString();
        m_status = "Error: " + errorString;
        emit statusChanged(m_status);
        emit errorOccurred(errorString);
    }
}

void CanBusController::handleStateChanged(QCanBusDevice::CanBusDeviceState state)
{
    switch (state) {
    case QCanBusDevice::ConnectedState:
        m_connected = true;
        m_status = "Connected to CAN Bus";
        break;
    case QCanBusDevice::ConnectingState:
        m_status = "Connecting to CAN Bus";
        break;
    case QCanBusDevice::UnconnectedState:
        m_connected = false;
        m_status = "Disconnected from CAN Bus";
        break;
    }
    
    emit connectedChanged(m_connected);
    emit statusChanged(m_status);
}
#endif

void CanBusController::simulateVehicleData()
{
    // Simulate realistic vehicle behavior
    QRandomGenerator *rng = QRandomGenerator::global();
    
    // Speed variation (0-120 km/h)
    int speedChange = rng->bounded(-2, 3);
    m_speed = qBound(0, m_speed + speedChange, 120);
    
    // RPM correlates with speed
    int targetRpm = 800 + (m_speed * 25); // Idle + speed-based RPM
    m_rpm = qBound(700, targetRpm + rng->bounded(-100, 101), 6000);
    
    // Fuel consumption (very slow decrease)
    if (m_speed > 0 && rng->bounded(0, 1000) < 1) {
        m_fuelLevel = qMax(0, m_fuelLevel - 1);
    }
    
    // Engine temperature (stable around 90°C)
    int tempChange = rng->bounded(-1, 2);
    m_engineTemp = qBound(70, m_engineTemp + tempChange, 110);
    
    // Random turn signals
    if (rng->bounded(0, 100) < 2) {
        m_leftTurnSignal = !m_leftTurnSignal;
        m_rightTurnSignal = false;
    }
    if (rng->bounded(0, 100) < 2) {
        m_rightTurnSignal = !m_rightTurnSignal;
        m_leftTurnSignal = false;
    }
    
    // Headlights based on time simulation
    static int timeCounter = 0;
    timeCounter++;
    if (timeCounter % 300 == 0) { // Change every 30 seconds
        m_headlights = !m_headlights;
    }

    // Emit CAN frames with simulated data
    QByteArray speedData;
    speedData.append(static_cast<char>(m_speed));
    speedData.append(static_cast<char>(m_speed >> 8));
    emit frameReceived(0x100, speedData); // Speed frame

    QByteArray rpmData;
    rpmData.append(static_cast<char>(m_rpm));
    rpmData.append(static_cast<char>(m_rpm >> 8));
    emit frameReceived(0x101, rpmData); // RPM frame

    QByteArray fuelData;
    fuelData.append(static_cast<char>(m_fuelLevel));
    emit frameReceived(0x102, fuelData); // Fuel level frame

    QByteArray tempData;
    tempData.append(static_cast<char>(m_engineTemp));
    emit frameReceived(0x103, tempData); // Engine temperature frame

    // Turn signals and lights
    QByteArray signalsData;
    quint8 signalBits = 0;
    if (m_leftTurnSignal) signalBits |= 0x01;
    if (m_rightTurnSignal) signalBits |= 0x02;
    if (m_headlights) signalBits |= 0x04;
    signalsData.append(static_cast<char>(signalBits));
    emit frameReceived(0x104, signalsData); // Signals frame
}

void CanBusController::setupSimulatedData()
{
    // Initialize with realistic starting values
    m_speed = 0;
    m_rpm = 800; // Idle RPM
    m_fuelLevel = 85; // 85% fuel
    m_engineTemp = 90; // Normal operating temperature
    m_leftTurnSignal = false;
    m_rightTurnSignal = false;
    m_headlights = false;
}