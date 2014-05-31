#ifndef I2CUtils_h
#define I2CUtils_h

#include <Arduino.h>

class I2CUtils
{
public:
    static uint8_t read8(int deviceAddress, uint8_t a);
    static uint16_t read16(int deviceAddress, uint8_t a);
    static void write8(int deviceAddress, uint8_t a, uint8_t d);
};

#endif // I2CUtils_h
