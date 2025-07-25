#ifndef HVACHANDLER_H
#define HVACHANDLER_H

#include <QObject>

/**
 * @brief The HvacHandler class manages temperature control for vehicle climate system.
 *
 * This class handles the target temperature setting and provides methods to increment
 * and decrement the temperature. It uses Qt's property system to expose values to QML.
 */
class HvacHandler : public QObject
{
    Q_OBJECT
    
    // Exposes the targetTemperature property to QML
    Q_PROPERTY(int targetTemperature READ targetTemperature WRITE setTargetTemperature NOTIFY targetTemperatureChanged)

public:
    /**
     * @brief Constructs an HvacHandler object.
     * @param parent The parent QObject, typically null for a top-level object.
     */
    explicit HvacHandler(QObject *parent = nullptr);

    /**
     * @brief Gets the target temperature.
     * @return The current target temperature value.
     */
    int targetTemperature() const;

public slots:
    /**
     * @brief Sets the target temperature.
     * Emits the targetTemperatureChanged signal if the value changes.
     * @param targetTemperature The new target temperature.
     */
    void setTargetTemperature(int targetTemperature);

    /**
     * @brief Increments or decrements the target temperature by the specified value.
     * @param val The value to add to the current temperature (can be negative for decrement).
     */
    Q_INVOKABLE void incrementTargetTemperature(int val);

signals:
    /**
     * @brief Signal emitted when the target temperature changes.
     * @param targetTemperature The new target temperature.
     */
    void targetTemperatureChanged(int targetTemperature);

private:
    int m_targetTemperature;
};

#endif // HVACHANDLER_H