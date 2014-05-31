#include <I2CUtils.h>
#include <Wire.h>

uint8_t I2CUtils::read8(int deviceAddress, uint8_t a)
{
    uint8_t ret;

    Wire.beginTransmission(deviceAddress); // start transmission to device 
    Wire.write(a); // sends register address to read from
    Wire.endTransmission(); // end transmission

    Wire.beginTransmission(deviceAddress); // start transmission to device 
    Wire.requestFrom(deviceAddress, 1);// send data n-bytes read
    ret = Wire.read(); // receive DATA
    Wire.endTransmission(); // end transmission

    return ret;
}

uint16_t I2CUtils::read16(int deviceAddress, uint8_t a)
{
    uint16_t ret;

    Wire.beginTransmission(deviceAddress); // start transmission to device 
    Wire.write(a); // sends register address to read from
    Wire.endTransmission(); // end transmission

    Wire.beginTransmission(deviceAddress); // start transmission to device 
    Wire.requestFrom(deviceAddress, 2);// send data n-bytes read
    ret = Wire.read(); // receive DATA
    ret <<= 8;
    ret |= Wire.read(); // receive DATA
    Wire.endTransmission(); // end transmission

    return ret;
}

void I2CUtils::write8(int deviceAddress, uint8_t a, uint8_t d)
{
    Wire.beginTransmission(deviceAddress); // start transmission to device 
    Wire.write(a); // sends register address to read from
    Wire.write(d);  // write data
    Wire.endTransmission(); // end transmission
}
