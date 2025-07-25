#include "audiocontroller.h"

AudioController::AudioController(QObject *parent)
    : QObject(parent), m_volumeLevel(51)
{
    // Initialize with a default volume level of 51 (medium level)
}

int AudioController::volumeLevel() const
{
    return m_volumeLevel;
}

void AudioController::setVolumeLevel(int volumeLevel)
{
    if (m_volumeLevel != volumeLevel) {
        m_volumeLevel = volumeLevel;
        emit volumeLevelChanged(m_volumeLevel);
    }
}

void AudioController::incrementVolume(int val)
{
    int newVolume = m_volumeLevel + val;
    
    // Constrain volume within bounds (0-100)
    if (newVolume <= 0) {
        newVolume = 0;
    } else if (newVolume >= 100) {
        newVolume = 100;
    }
    
    setVolumeLevel(newVolume);
}