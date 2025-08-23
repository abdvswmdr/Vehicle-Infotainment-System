#include "vehicledatacontroller.h"
#include <QDebug>

VehicleDataController::VehicleDataController(QObject *parent)
    : QObject(parent)
    , m_speed(0)
    , m_rpm(0)
    , m_fuelLevel(100)
    , m_engineTemperature(70)
    , m_leftTurnSignal(false)
    , m_rightTurnSignal(false)
    , m_headlights(false)
    , m_parkingBrake(true)
    , m_gear("P")
    , m_odometer(12345.6)
    , m_tripOdometer(0.0)
    , m_batteryVoltage(12)
    , m_engineRunning(false)
    , m_seatbelt(false)
    , m_doorOpen(false)
    , m_odometerTimer(new QTimer(this))
    , m_previousSpeed(0)
{
    connect(m_odometerTimer, &QTimer::timeout, this, &VehicleDataController::updateOdometer);
    m_odometerTimer->start(1000); // Update odometer every second
}

// Getters
int VehicleDataController::speed() const { return m_speed; }
int VehicleDataController::rpm() const { return m_rpm; }
int VehicleDataController::fuelLevel() const { return m_fuelLevel; }
int VehicleDataController::engineTemperature() const { return m_engineTemperature; }
bool VehicleDataController::leftTurnSignal() const { return m_leftTurnSignal; }
bool VehicleDataController::rightTurnSignal() const { return m_rightTurnSignal; }
bool VehicleDataController::headlights() const { return m_headlights; }
bool VehicleDataController::parkingBrake() const { return m_parkingBrake; }
QString VehicleDataController::gear() const { return m_gear; }
double VehicleDataController::odometer() const { return m_odometer; }
int VehicleDataController::batteryVoltage() const { return m_batteryVoltage; }
bool VehicleDataController::engineRunning() const { return m_engineRunning; }
bool VehicleDataController::seatbelt() const { return m_seatbelt; }
bool VehicleDataController::doorOpen() const { return m_doorOpen; }

void VehicleDataController::processCanFrame(quint32 frameId, const QByteArray &data)
{
    if (data.isEmpty()) {
        return;
    }

    switch (frameId) {
    case 0x100: // Speed frame
        if (data.size() >= 2) {
            int speed = static_cast<quint8>(data[0]) | (static_cast<quint8>(data[1]) << 8);
            setSpeed(speed);
        }
        break;
        
    case 0x101: // RPM frame
        if (data.size() >= 2) {
            int rpm = static_cast<quint8>(data[0]) | (static_cast<quint8>(data[1]) << 8);
            setRpm(rpm);
            
            // Update engine running state based on RPM
            bool wasRunning = m_engineRunning;
            bool running = rpm > 500;
            if (running != wasRunning) {
                setEngineRunning(running);
            }
        }
        break;
        
    case 0x102: // Fuel level frame
        if (data.size() >= 1) {
            int fuelLevel = static_cast<quint8>(data[0]);
            setFuelLevel(fuelLevel);
        }
        break;
        
    case 0x103: // Engine temperature frame
        if (data.size() >= 1) {
            int temp = static_cast<quint8>(data[0]);
            setEngineTemperature(temp);
        }
        break;
        
    case 0x104: // Turn signals and lights frame
        if (data.size() >= 1) {
            quint8 signals = static_cast<quint8>(data[0]);
            setLeftTurnSignal(signals & 0x01);
            setRightTurnSignal(signals & 0x02);
            setHeadlights(signals & 0x04);
        }
        break;
        
    case 0x105: // Vehicle status frame
        if (data.size() >= 2) {
            quint8 status1 = static_cast<quint8>(data[0]);
            quint8 status2 = static_cast<quint8>(data[1]);
            
            setParkingBrake(status1 & 0x01);
            setSeatbelt(status1 & 0x02);
            setDoorOpen(status1 & 0x04);
            
            // Gear calculation from status2
            int gearValue = status2 & 0x0F;
            QString gearStr;
            switch (gearValue) {
            case 0: gearStr = "P"; break;
            case 1: gearStr = "R"; break;
            case 2: gearStr = "N"; break;
            case 3: gearStr = "D"; break;
            case 4: gearStr = "D2"; break;
            case 5: gearStr = "D3"; break;
            default: gearStr = "?"; break;
            }
            setGear(gearStr);
        }
        break;
        
    case 0x106: // Battery voltage frame
        if (data.size() >= 1) {
            int voltage = static_cast<quint8>(data[0]);
            setBatteryVoltage(voltage);
        }
        break;
        
    default:
        qDebug() << "Unknown CAN frame ID:" << Qt::hex << frameId;
        break;
    }
}

void VehicleDataController::resetTripOdometer()
{
    m_tripOdometer = 0.0;
}

void VehicleDataController::toggleEngineState()
{
    if (m_engineRunning) {
        setRpm(0);
        setEngineRunning(false);
    } else {
        setRpm(800); // Idle RPM
        setEngineRunning(true);
    }
}

void VehicleDataController::updateOdometer()
{
    if (m_speed > 0) {
        // Convert km/h to km per second, then add to odometer
        double distanceIncrement = (m_speed / 3600.0); // km/h to km/s
        m_odometer += distanceIncrement;
        m_tripOdometer += distanceIncrement;
        emit odometerChanged(m_odometer);
    }
}

// Private setters with signal emission and warning checks
void VehicleDataController::setSpeed(int speed)
{
    if (m_speed != speed) {
        m_speed = speed;
        emit speedChanged(m_speed);
    }
}

void VehicleDataController::setRpm(int rpm)
{
    if (m_rpm != rpm) {
        m_rpm = rpm;
        emit rpmChanged(m_rpm);
    }
}

void VehicleDataController::setFuelLevel(int fuelLevel)
{
    if (m_fuelLevel != fuelLevel) {
        m_fuelLevel = fuelLevel;
        emit fuelLevelChanged(m_fuelLevel);
        
        // Low fuel warning
        if (m_fuelLevel <= 10) {
            emit lowFuelWarning();
        }
    }
}

void VehicleDataController::setEngineTemperature(int engineTemperature)
{
    if (m_engineTemperature != engineTemperature) {
        m_engineTemperature = engineTemperature;
        emit engineTemperatureChanged(m_engineTemperature);
        
        // Overheat warning
        if (m_engineTemperature >= 105) {
            emit engineOverheatWarning();
        }
    }
}

void VehicleDataController::setLeftTurnSignal(bool leftTurnSignal)
{
    if (m_leftTurnSignal != leftTurnSignal) {
        m_leftTurnSignal = leftTurnSignal;
        emit leftTurnSignalChanged(m_leftTurnSignal);
    }
}

void VehicleDataController::setRightTurnSignal(bool rightTurnSignal)
{
    if (m_rightTurnSignal != rightTurnSignal) {
        m_rightTurnSignal = rightTurnSignal;
        emit rightTurnSignalChanged(m_rightTurnSignal);
    }
}

void VehicleDataController::setHeadlights(bool headlights)
{
    if (m_headlights != headlights) {
        m_headlights = headlights;
        emit headlightsChanged(m_headlights);
    }
}

void VehicleDataController::setParkingBrake(bool parkingBrake)
{
    if (m_parkingBrake != parkingBrake) {
        m_parkingBrake = parkingBrake;
        emit parkingBrakeChanged(m_parkingBrake);
    }
}

void VehicleDataController::setGear(const QString &gear)
{
    if (m_gear != gear) {
        m_gear = gear;
        emit gearChanged(m_gear);
    }
}

void VehicleDataController::setBatteryVoltage(int batteryVoltage)
{
    if (m_batteryVoltage != batteryVoltage) {
        m_batteryVoltage = batteryVoltage;
        emit batteryVoltageChanged(m_batteryVoltage);
        
        // Low battery warning
        if (m_batteryVoltage <= 11) {
            emit batteryLowWarning();
        }
    }
}

void VehicleDataController::setEngineRunning(bool engineRunning)
{
    if (m_engineRunning != engineRunning) {
        m_engineRunning = engineRunning;
        emit engineRunningChanged(m_engineRunning);
    }
}

void VehicleDataController::setSeatbelt(bool seatbelt)
{
    if (m_seatbelt != seatbelt) {
        m_seatbelt = seatbelt;
        emit seatbeltChanged(m_seatbelt);
    }
}

void VehicleDataController::setDoorOpen(bool doorOpen)
{
    if (m_doorOpen != doorOpen) {
        m_doorOpen = doorOpen;
        emit doorOpenChanged(m_doorOpen);
    }
}