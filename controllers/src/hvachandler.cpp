#include "hvachandler.h"

HvacHandler::HvacHandler(QObject *parent)
    : QObject(parent), m_targetTemperature(70)
{
    // Initialize with a default comfortable temperature of 70°F
}

int HvacHandler::targetTemperature() const
{
    return m_targetTemperature;
}

void HvacHandler::setTargetTemperature(int targetTemperature)
{
    if (m_targetTemperature != targetTemperature) {
        m_targetTemperature = targetTemperature;
        emit targetTemperatureChanged(m_targetTemperature);
    }
}

void HvacHandler::incrementTargetTemperature(int val)
{
    int newTargetTemp = m_targetTemperature + val;
    
    // Constrain temperature within reasonable bounds (50-90°F)
    if (newTargetTemp < 50) {
        newTargetTemp = 50;
    } else if (newTargetTemp > 90) {
        newTargetTemp = 90;
    }
    
    setTargetTemperature(newTargetTemp);
}