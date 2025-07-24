#ifndef SYSTEM_H
#define SYSTEM_H

#include <QObject>
#include <QString>

/**
 * @brief The System class represents the backend system for the application.
 *
 * It manages various properties of the system like car lock status,
 * outdoor temperature, and the user's name. It uses Qt's property
 * system to expose these values to QML or other parts of a Qt application.
 */
class System : public QObject
{
    Q_OBJECT
    // Exposes the carLocked property to QML and other meta-object system clients.
    // READ: Specifies the getter function.
    // WRITE: Specifies the setter function.
    // NOTIFY: Specifies the signal that is emitted when the value changes.
    Q_PROPERTY(bool carLocked READ carLocked WRITE setCarLocked NOTIFY carLockedChanged)
    Q_PROPERTY(int outdoorTemp READ outdoorTemp WRITE setOutdoorTemp NOTIFY outdoorTempChanged)
    Q_PROPERTY(QString userName READ userName WRITE setUserName NOTIFY userNameChanged)

public:
    /**
     * @brief Constructs a System object.
     * @param parent The parent QObject, typically null for a top-level object.
     */
    explicit System(QObject *parent = nullptr);

    // --- Getters ---
    /**
     * @brief Gets the current lock status of the car.
     * @return True if the car is locked, false otherwise.
     */
    bool carLocked() const;

    /**
     * @brief Gets the current outdoor temperature.
     * @return The temperature value.
     */
    int outdoorTemp() const;

    /**
     * @brief Gets the current user's name.
     * @return The user's name as a QString.
     */
    QString userName() const;


public slots:
    // --- Setters ---
    /**
     * @brief Sets the lock status of the car.
     * Emits the carLockedChanged signal if the value changes.
     * @param carLocked The new lock status.
     */
    void setCarLocked(bool carLocked);

    /**
     * @brief Sets the outdoor temperature.
     * Emits the outdoorTempChanged signal if the value changes.
     * @param outdoorTemp The new temperature.
     */
    void setOutdoorTemp(int outdoorTemp);

    /**
     * @brief Sets the user's name.
     * Emits the userNameChanged signal if the value changes.
     * @param userName The new user name.
     */
    void setUserName(const QString &userName);

signals:
    /**
     * @brief Signal emitted when the car's lock status changes.
     * @param carLocked The new lock status.
     */
    void carLockedChanged(bool carLocked);

    /**
     * @brief Signal emitted when the outdoor temperature changes.
     * @param outdoorTemp The new temperature.
     */
    void outdoorTempChanged(int outdoorTemp);

    /**
     * @brief Signal emitted when the user's name changes.
     * @param userName The new user name.
     */
    void userNameChanged(const QString &userName);

private:
    // Member variables to store the property values.
    bool m_carLocked;
    int m_outdoorTemp;
    QString m_userName;
};

#endif // SYSTEM_H
