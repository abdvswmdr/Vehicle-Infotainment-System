#ifndef AUDIOCONTROLLER_H
#define AUDIOCONTROLLER_H

#include <QObject>

/**
 * @brief The AudioController class manages volume control for the vehicle audio system.
 *
 * This class handles the volume level setting and provides methods to increment
 * and decrement the volume. It uses Qt's property system to expose values to QML.
 */
class AudioController : public QObject
{
    Q_OBJECT
    
    // Exposes the volumeLevel property to QML
    Q_PROPERTY(int volumeLevel READ volumeLevel WRITE setVolumeLevel NOTIFY volumeLevelChanged)

public:
    /**
     * @brief Constructs an AudioController object.
     * @param parent The parent QObject, typically null for a top-level object.
     */
    explicit AudioController(QObject *parent = nullptr);

    /**
     * @brief Gets the volume level.
     * @return The current volume level value (0-100).
     */
    int volumeLevel() const;

public slots:
    /**
     * @brief Sets the volume level.
     * Emits the volumeLevelChanged signal if the value changes.
     * @param volumeLevel The new volume level (0-100).
     */
    void setVolumeLevel(int volumeLevel);

    /**
     * @brief Increments or decrements the volume level by the specified value.
     * @param val The value to add to the current volume (can be negative for decrement).
     */
    Q_INVOKABLE void incrementVolume(int val);

signals:
    /**
     * @brief Signal emitted when the volume level changes.
     * @param volumeLevel The new volume level.
     */
    void volumeLevelChanged(int volumeLevel);

private:
    int m_volumeLevel;
};

#endif // AUDIOCONTROLLER_H