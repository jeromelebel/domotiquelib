#ifndef SunblindDriver_h
#define SunblindDriver_h

#define SUNBLIND_DRIVER_PIN_COUNT          3

#include <Arduino.h>

class SunblindDriver
{
    byte                        _pins[SUNBLIND_DRIVER_PIN_COUNT];
    byte                        _shouldReset;
    unsigned long               _lastCommand;
    
    void sendCommand(byte command);
    
public:
    SunblindDriver(byte upPin, byte stopPin, byte downPin);
    
    boolean loop(void);
    void sendUp(void);
    void sendStop(void);
    void sendDown(void);
};

#endif // SunblindDriver_h
