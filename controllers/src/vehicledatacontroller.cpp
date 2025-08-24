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
    case 0x100: // Engine_Data (256 decimal) - Engine speed, load, temperature, fuel
        if (data.size() >= 8) {
            // Engine Speed (RPM) - bytes 0-1, scale 0.25
            int rpm = (static_cast<quint8>(data[0]) | (static_cast<quint8>(data[1]) << 8)) * 0.25;
            setRpm(rpm);
            
            // Engine running state based on RPM
            bool wasRunning = m_engineRunning;
            bool running = rpm > 500;
            if (running != wasRunning) {
                setEngineRunning(running);
            }
            
            // Engine Coolant Temperature - byte 3, scale 1, offset -40
            if (data.size() >= 4) {
                int temp = static_cast<quint8>(data[3]) - 40;
                setEngineTemperature(temp);
            }
            
            // Fuel Level - byte 7, scale 0.392157 (percent)
            if (data.size() >= 8) {
                int fuelLevel = static_cast<quint8>(data[7]) * 0.392157;
                setFuelLevel(fuelLevel);
            }
        }
        break;
        
    case 0x200: // Vehicle_Speed (512 decimal) - Vehicle speed
        if (data.size() >= 2) {
            // Vehicle Speed - bytes 0-1, scale 0.1 km/h
            int speed = (static_cast<quint8>(data[0]) | (static_cast<quint8>(data[1]) << 8)) * 0.1;
            setSpeed(speed);
        }
        break;
        
    case 0x400: // Transmission_Data (1024 decimal) - Gear position
        if (data.size() >= 3) {
            // Gear Position - lower 4 bits of byte 0
            int gearValue = static_cast<quint8>(data[0]) & 0x0F;
            QString gearStr;
            switch (gearValue) {
            case 0: gearStr = "P"; break;
            case 1: gearStr = "R"; break;
            case 2: gearStr = "N"; break;
            case 3: gearStr = "D"; break;
            case 4: gearStr = "S"; break; // Sport mode
            case 5: gearStr = "M1"; break; // Manual 1st
            case 6: gearStr = "M2"; break; // Manual 2nd
            case 7: gearStr = "M3"; break; // Manual 3rd
            case 8: gearStr = "M4"; break; // Manual 4th
            case 9: gearStr = "M5"; break; // Manual 5th
            case 10: gearStr = "M6"; break; // Manual 6th
            default: gearStr = "?"; break;
            }
            setGear(gearStr);
            
            // Park status from bit 1 of byte 2
            if (data.size() >= 3) {
                quint8 status = static_cast<quint8>(data[2]);
                setParkingBrake((status >> 1) & 0x01);
            }
        }
        break;
        
    case 0x500: // Battery_Status (1280 decimal) - Battery voltage
        if (data.size() >= 3) {
            // Battery Voltage - bytes 0-1, scale 0.01 V
            int voltage = (static_cast<quint8>(data[0]) | (static_cast<quint8>(data[1]) << 8)) * 0.01;
            setBatteryVoltage(voltage);
        }
        break;
        
    case 0x600: // Warning_Lights (1536 decimal) - Turn signals and indicators
        if (data.size() >= 2) {
            quint8 warnings = static_cast<quint8>(data[0]);
            quint8 signalBits = static_cast<quint8>(data[1]);
            
            // Turn signals from byte 1
            setLeftTurnSignal(signalBits & 0x01);
            setRightTurnSignal((signalBits >> 1) & 0x01);
            setHeadlights((signalBits >> 2) & 0x01);
        }
        break;
        
    case 0x700: // Door_Status (1792 decimal) - Door and closure status
        if (data.size() >= 1) {
            quint8 doorStatus = static_cast<quint8>(data[0]);
            // Set door open if any door is open
            bool anyDoorOpen = (doorStatus & 0x0F) != 0; // Check first 4 bits (doors)
            setDoorOpen(anyDoorOpen);
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