#ifndef AnalogSensor_h
#define AnalogSensor_h

#include "Arduino.h"
#include "Sensor.h"

class AnalogSensor : public Sensor {
    private:
        const char          *_type;
        unsigned int        _lastValue;
        unsigned long       _valueSum;
        unsigned int        _valueCount;
        uint8_t             _pin;
        unsigned long       _millisDelay;
        unsigned long       _lastValueTime;
        unsigned short       _trigger;

    public:
        AnalogSensor(const char *type, uint8_t pin, unsigned long millisDelay, unsigned short trigger = 0);

        // Sensor
        const char *sensorClass(void) { return "Analog"; };
        const char *sensorType(void) { return _type; };
        boolean begin(void);
        boolean printValues(Stream *serial);
        boolean printAddress(Stream *serial);
        boolean loop(void);
        boolean resetValues(void);
};

#endif // AnalogSensor_h
