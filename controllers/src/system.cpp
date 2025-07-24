#include "system.h"

/**
 * @brief Constructor for the System class.
 *
 * Initializes the system with default values for its properties.
 * @param parent The parent QObject.
 */
System::System(QObject *parent)
    : QObject(parent)
    , m_carLocked(true)      // Default car status is locked
    , m_outdoorTemp(32)      // Default temperature
    , m_userName("User")     // Default user name
{
}

// --- Getter Implementations ---

bool System::carLocked() const
{
    return m_carLocked;
}

int System::outdoorTemp() const
{
    return m_outdoorTemp;
}

QString System::userName() const
{
    return m_userName;
}

// --- Setter Implementations ---

void System::setCarLocked(bool carLocked)
{
    // Check if the new value is the same as the current one.
    // If so, do nothing to avoid unnecessary signal emissions and potential loops.
    if (m_carLocked == carLocked)
        return;

    m_carLocked = carLocked;
    // Emit the corresponding 'changed' signal to notify listeners.
    emit carLockedChanged(m_carLocked);
}

void System::setOutdoorTemp(int outdoorTemp)
{
    if (m_outdoorTemp == outdoorTemp)
        return;

    m_outdoorTemp = outdoorTemp;
    emit outdoorTempChanged(m_outdoorTemp);
}

void System::setUserName(const QString &userName)
{
    if (m_userName == userName)
        return;

    m_userName = userName;
    emit userNameChanged(m_userName);
}
