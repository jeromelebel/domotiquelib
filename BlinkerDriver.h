#ifndef BlinkerDriver_h
#define BlinkerDriver_h

#include <Arduino.h>
#include <Driver.h>

class BlinkerDriver : Driver
{
protected:
    char                _ledPin;
    unsigned long       _lastTimeSwitch;
    int                 _currentState;
    unsigned int        _offStateTime;
    unsigned int        _onStateTime;
    
public:
    BlinkerDriver(char ledPin, unsigned int offStateTime = 1900, unsigned int onStateTime = 100);
  
    boolean loop(void);
};

#endif // BlinkerDriver_h
