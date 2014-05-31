#ifndef Sensor_h
#define Sensor_h

#include "Arduino.h"

class Sensor
{
public:
    virtual const char *sensorClass(void) = 0;
    virtual const char *sensorType(void) { return ""; };
    virtual const char *addressString(void) { return ""; };
    virtual const char *copyAddressString(void) { return NULL; };
    virtual boolean begin(void) { return true; };
    virtual boolean isReady(void) { return true; };
    virtual boolean loop(void) { return true; };
    virtual boolean readValues(void) { return true; };
    virtual boolean resetValues(void) { return true; };
    virtual boolean printInfo(Stream *serial, int nodeID = -1);
    virtual boolean printAddress(Stream *serial);
    virtual boolean printValues(Stream *serial) { return false; };
};

#endif // Sensor_h
