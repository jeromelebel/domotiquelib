#ifndef LowBatterySensor_h
#define LowBatterySensor_h

#include <Arduino.h>
#include "Sensor.h"

class LowBatterySensor : public Sensor {
    public:
        // Sensor
        const char *sensorClass(void) { return "LowBattery"; };
        boolean printValues(Stream *serial);
        boolean printAddress(Stream *serial);
};

#endif // LowBatterySensor_h
