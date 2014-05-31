#include "BlinkerDriver.h"

BlinkerDriver::BlinkerDriver(char ledPin, unsigned int offStateTime, unsigned int onStateTime)
{
    _ledPin = ledPin;
    _currentState = LOW;
    _onStateTime = onStateTime;
    _offStateTime = offStateTime;
    pinMode(_ledPin, OUTPUT);
}

boolean BlinkerDriver::loop(void)
{
    char stateChanged = 0;
    unsigned long currentDate = millis();

    if (_currentState == LOW && (unsigned long)(currentDate - _lastTimeSwitch) > _offStateTime) {
        _currentState = HIGH;
        stateChanged = 1;
    } else if (_currentState == HIGH && (unsigned long)(currentDate - _lastTimeSwitch) > _onStateTime) {
        _currentState = LOW;
        stateChanged = 1;
    }
    if (stateChanged == 1) {
        _lastTimeSwitch = currentDate;
        digitalWrite(_ledPin, _currentState);
    }
    return true;
}
