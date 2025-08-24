#ifndef VEHICLEDATACONTROLLER_H
#define VEHICLEDATACONTROLLER_H

#include <QObject>
#include <QString>
#include <QTimer>

class VehicleDataController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int speed READ speed NOTIFY speedChanged)
    Q_PROPERTY(int rpm READ rpm NOTIFY rpmChanged)
    Q_PROPERTY(int fuelLevel READ fuelLevel NOTIFY fuelLevelChanged)
    Q_PROPERTY(int engineTemperature READ engineTemperature NOTIFY engineTemperatureChanged)
    Q_PROPERTY(bool leftTurnSignal READ leftTurnSignal NOTIFY leftTurnSignalChanged)
    Q_PROPERTY(bool rightTurnSignal READ rightTurnSignal NOTIFY rightTurnSignalChanged)
    Q_PROPERTY(bool headlights READ headlights NOTIFY headlightsChanged)
    Q_PROPERTY(bool parkingBrake READ parkingBrake NOTIFY parkingBrakeChanged)
    Q_PROPERTY(QString gear READ gear NOTIFY gearChanged)
    Q_PROPERTY(double odometer READ odometer NOTIFY odometerChanged)
    Q_PROPERTY(int batteryVoltage READ batteryVoltage NOTIFY batteryVoltageChanged)
    Q_PROPERTY(bool engineRunning READ engineRunning NOTIFY engineRunningChanged)
    Q_PROPERTY(bool seatbelt READ seatbelt NOTIFY seatbeltChanged)
    Q_PROPERTY(bool doorOpen READ doorOpen NOTIFY doorOpenChanged)

public:
    explicit VehicleDataController(QObject *parent = nullptr);

    // Getters
    int speed() const;
    int rpm() const;
    int fuelLevel() const;
    int engineTemperature() const;
    bool leftTurnSignal() const;
    bool rightTurnSignal() const;
    bool headlights() const;
    bool parkingBrake() const;
    QString gear() const;
    double odometer() const;
    int batteryVoltage() const;
    bool engineRunning() const;
    bool seatbelt() const;
    bool doorOpen() const;

public slots:
    void processCanFrame(quint32 frameId, const QByteArray &data);
    void resetTripOdometer();
    void toggleEngineState();

signals:
    void speedChanged(int speed);
    void rpmChanged(int rpm);
    void fuelLevelChanged(int fuelLevel);
    void engineTemperatureChanged(int engineTemperature);
    void leftTurnSignalChanged(bool leftTurnSignal);
    void rightTurnSignalChanged(bool rightTurnSignal);
    void headlightsChanged(bool headlights);
    void parkingBrakeChanged(bool parkingBrake);
    void gearChanged(const QString &gear);
    void odometerChanged(double odometer);
    void batteryVoltageChanged(int batteryVoltage);
    void engineRunningChanged(bool engineRunning);
    void seatbeltChanged(bool seatbelt);
    void doorOpenChanged(bool doorOpen);
    
    // Warning signals
    void lowFuelWarning();
    void engineOverheatWarning();
    void batteryLowWarning();

private slots:
    void updateOdometer();

private:
    void setSpeed(int speed);
    void setRpm(int rpm);
    void setFuelLevel(int fuelLevel);
    void setEngineTemperature(int engineTemperature);
    void setLeftTurnSignal(bool leftTurnSignal);
    void setRightTurnSignal(bool rightTurnSignal);
    void setHeadlights(bool headlights);
    void setParkingBrake(bool parkingBrake);
    void setGear(const QString &gear);
    void setBatteryVoltage(int batteryVoltage);
    void setEngineRunning(bool engineRunning);
    void setSeatbelt(bool seatbelt);
    void setDoorOpen(bool doorOpen);

    // Vehicle state variables
    int m_speed;
    int m_rpm;
    int m_fuelLevel;
    int m_engineTemperature;
    bool m_leftTurnSignal;
    bool m_rightTurnSignal;
    bool m_headlights;
    bool m_parkingBrake;
    QString m_gear;
    double m_odometer;
    double m_tripOdometer;
    int m_batteryVoltage;
    bool m_engineRunning;
    bool m_seatbelt;
    bool m_doorOpen;
    
    // Timers and helpers
    QTimer *m_odometerTimer;
    int m_previousSpeed;
};

#endif // VEHICLEDATACONTROLLER_H