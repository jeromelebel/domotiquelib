#include <LowBatterySensor.h>
#if !defined(__SAM3X8E__)
#include <RF12.h>
#endif

boolean LowBatterySensor::printValues(Stream *serial)
{
#if !defined(__SAM3X8E__)
    serial->print(rf12_lowbat()?"1":"0");
#endif
    return true;
}

boolean LowBatterySensor::printAddress(Stream *serial)
{
    return true;
}
