#include "AnalogSensor.h"

AnalogSensor::AnalogSensor(const char *type, uint8_t pin, unsigned long millisDelay, unsigned short trigger)
{
    _pin = pin;
    if (type == NULL) {
        type = "";
    }
    _type = type;
    _trigger = trigger;
    _millisDelay = millisDelay;
    this->resetValues();
}

boolean AnalogSensor::begin(void)
{
    pinMode(_pin, INPUT);
    digitalWrite(_pin, HIGH);
    return true;
}

boolean AnalogSensor::loop(void)
{
    unsigned long currentTime = millis();
    
    if ((unsigned long)(currentTime - _lastValueTime) >= _millisDelay) {
        _lastValueTime = currentTime;
        _lastValue = analogRead(_pin);
        if (_trigger > 0) {
            if (_trigger < _lastValue) {
                _lastValue = 0;
            } else {
                _lastValue = 1;
            }
        }
        _valueSum += _lastValue;
        _valueCount++;
    }
}

boolean AnalogSensor::printValues(Stream *serial)
{
    serial->print(_lastValue);
    serial->print(" ");
    serial->print((float)_valueSum / (float)_valueCount);
    serial->print(" ");
    serial->print(_valueSum);
    serial->print(" ");
    serial->print(_valueCount);
}

boolean AnalogSensor::printAddress(Stream *serial)
{
    serial->print(_pin);
}

boolean AnalogSensor::resetValues(void)
{
    _valueCount = 0;
    _valueSum = 0;
    _lastValue = 0;
    _lastValueTime = millis();
    return true;
}
