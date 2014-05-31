#include "SunblindDriver.h"

#define UP_PIN_INDEX            0
#define STOP_PIN_INDEX          1
#define DOWN_PIN_INDEX          2
#define DELAY                   1000

SunblindDriver::SunblindDriver(byte upPin, byte stopPin, byte downPin)
{
    byte ii;
    
    _pins[UP_PIN_INDEX] = upPin;
    _pins[STOP_PIN_INDEX] = stopPin;
    _pins[DOWN_PIN_INDEX] = downPin;
    for (ii = 0; ii < SUNBLIND_DRIVER_PIN_COUNT; ii++) {
        pinMode(_pins[ii], OUTPUT);
    }
}

boolean SunblindDriver::loop(void)
{
    if (_shouldReset == 1) {
        unsigned long currentTime = millis();
        
        if ((unsigned long)(currentTime - _lastCommand) >= DELAY) {
            this->sendCommand(-1);
        }
    }
    return true;
}

void SunblindDriver::sendCommand(byte command)
{
    byte ii;
    
    for (ii = 0; ii < SUNBLIND_DRIVER_PIN_COUNT; ii++) {
        digitalWrite(_pins[ii], (command == ii)?HIGH:LOW);
    }
    if (command == -1) {
        _shouldReset = 0;
    } else {
        _shouldReset = 1;
        _lastCommand = millis();
    }
}

void SunblindDriver::sendUp(void)
{
    this->sendCommand(UP_PIN_INDEX);
}

void SunblindDriver::sendStop(void)
{
    this->sendCommand(STOP_PIN_INDEX);
}

void SunblindDriver::sendDown(void)
{
    this->sendCommand(DOWN_PIN_INDEX);
}
