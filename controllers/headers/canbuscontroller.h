#ifndef CANBUSCONTROLLER_H
#define CANBUSCONTROLLER_H

#include <QObject>
#include <QCanBusDevice>
#include <QCanBusFrame>
#include <QTimer>
#include <QString>

class CanBusController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool connected READ connected NOTIFY connectedChanged)
    Q_PROPERTY(QString status READ status NOTIFY statusChanged)

public:
    explicit CanBusController(QObject *parent = nullptr);
    ~CanBusController();

    bool connected() const;
    QString status() const;

public slots:
    void connectToSimulator();
    void disconnectFromSimulator();
    void sendFrame(quint32 frameId, const QByteArray &data);

signals:
    void connectedChanged(bool connected);
    void statusChanged(const QString &status);
    void frameReceived(quint32 frameId, const QByteArray &data);
    void errorOccurred(const QString &error);

private slots:
    void handleFramesReceived();
    void handleErrorOccurred(QCanBusDevice::CanBusError error);
    void handleStateChanged(QCanBusDevice::CanBusDeviceState state);
    void simulateVehicleData();

private:
    void setupSimulatedData();
    
    QCanBusDevice *m_canDevice;
    QTimer *m_simulationTimer;
    bool m_connected;
    QString m_status;
    
    // Simulated vehicle data counters
    int m_speed;
    int m_rpm;
    int m_fuelLevel;
    int m_engineTemp;
    bool m_leftTurnSignal;
    bool m_rightTurnSignal;
    bool m_headlights;
};

#endif // CANBUSCONTROLLER_H