#ifndef DHTSensor_h
#define DHTSensor_h

#include "Arduino.h"
#include "Sensor.h"

/* DHT library 

MIT license
written by Adafruit Industries
*/

// how many timing transitions we need to keep track of. 2 * number bits + extra
#define MAXTIMINGS 85

typedef enum {
    DHTSensorType_11,
    DHTSensorType_21,
    DHTSensorType_22,
    DHTSensorType_AM2301,
} DHTSensorType;

class DHTSensor : public Sensor {
    private:
        float               _temperature;
        float               _humidity;
        uint8_t             _pin;
        DHTSensorType       _type;
        unsigned long       _lastreadtime;

    public:
        DHTSensor(uint8_t pin, DHTSensorType type);
        float temperature(void) { return _temperature; };
        float humidity(void) { return _humidity; };

        // Sensor
        boolean begin(void);
        const char *sensorClass(void);
        const char *sensorType(void);
        boolean printValues(Stream *serial);
        boolean loop(void);
        boolean printAddress(Stream *serial) { serial->print(_pin); return true; };
};

#endif // DHTSensor_h
