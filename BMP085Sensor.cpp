/*************************************************** 
  This is a library for the BMP085 Barometric Pressure & Temp Sensor

  Designed specifically to work with the Adafruit BMP085 Breakout 
  ----> https://www.adafruit.com/products/391

  These displays use I2C to communicate, 2 pins are required to  
  interface
  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/

#include "BMP085Sensor.h"
#include <Wire.h>
#include <I2CUtils.h>

#define BMP085_DEBUG 0

#define BMP085_I2CADDR 0x77

#define BMP085_CAL_AC1           0xAA  // R   Calibration data (16 bits)
#define BMP085_CAL_AC2           0xAC  // R   Calibration data (16 bits)
#define BMP085_CAL_AC3           0xAE  // R   Calibration data (16 bits)    
#define BMP085_CAL_AC4           0xB0  // R   Calibration data (16 bits)
#define BMP085_CAL_AC5           0xB2  // R   Calibration data (16 bits)
#define BMP085_CAL_AC6           0xB4  // R   Calibration data (16 bits)
#define BMP085_CAL_B1            0xB6  // R   Calibration data (16 bits)
#define BMP085_CAL_B2            0xB8  // R   Calibration data (16 bits)
#define BMP085_CAL_MB            0xBA  // R   Calibration data (16 bits)
#define BMP085_CAL_MC            0xBC  // R   Calibration data (16 bits)
#define BMP085_CAL_MD            0xBE  // R   Calibration data (16 bits)

#define BMP085_CONTROL           0xF4 
#define BMP085_TEMPDATA          0xF6
#define BMP085_PRESSUREDATA      0xF6
#define BMP085_READTEMPCMD       0x2E
#define BMP085_READPRESSURECMD   0x34

BMP085Sensor::BMP085Sensor(BMP085Sensor::SamplingMode samplingMode)
{
    _samplingMode = samplingMode;
}

const char *BMP085Sensor::sensorClass(void)
{
    return "BMP085";
}

boolean BMP085Sensor::begin(void)
{
    Wire.begin();

    if (I2CUtils::read8(BMP085_I2CADDR, 0xD0) != 0x55) return false;

    /* read calibration data */
    ac1 = I2CUtils::read16(BMP085_I2CADDR, BMP085_CAL_AC1);
    ac2 = I2CUtils::read16(BMP085_I2CADDR, BMP085_CAL_AC2);
    ac3 = I2CUtils::read16(BMP085_I2CADDR, BMP085_CAL_AC3);
    ac4 = I2CUtils::read16(BMP085_I2CADDR, BMP085_CAL_AC4);
    ac5 = I2CUtils::read16(BMP085_I2CADDR, BMP085_CAL_AC5);
    ac6 = I2CUtils::read16(BMP085_I2CADDR, BMP085_CAL_AC6);

    b1 = I2CUtils::read16(BMP085_I2CADDR, BMP085_CAL_B1);
    b2 = I2CUtils::read16(BMP085_I2CADDR, BMP085_CAL_B2);

    mb = I2CUtils::read16(BMP085_I2CADDR, BMP085_CAL_MB);
    mc = I2CUtils::read16(BMP085_I2CADDR, BMP085_CAL_MC);
    md = I2CUtils::read16(BMP085_I2CADDR, BMP085_CAL_MD);
#if (BMP085_DEBUG == 1)
    Serial.print("ac1 = "); Serial.println(ac1, DEC);
    Serial.print("ac2 = "); Serial.println(ac2, DEC);
    Serial.print("ac3 = "); Serial.println(ac3, DEC);
    Serial.print("ac4 = "); Serial.println(ac4, DEC);
    Serial.print("ac5 = "); Serial.println(ac5, DEC);
    Serial.print("ac6 = "); Serial.println(ac6, DEC);

    Serial.print("b1 = "); Serial.println(b1, DEC);
    Serial.print("b2 = "); Serial.println(b2, DEC);

    Serial.print("mb = "); Serial.println(mb, DEC);
    Serial.print("mc = "); Serial.println(mc, DEC);
    Serial.print("md = "); Serial.println(md, DEC);
#endif
}

void BMP085Sensor::readRawTemperature(void)
{
    I2CUtils::write8(BMP085_I2CADDR, BMP085_CONTROL, BMP085_READTEMPCMD);
    delay(5);
#if BMP085_DEBUG == 1
    Serial.print("Raw temp: "); Serial.println(I2CUtils::read16(BMP085_I2CADDR, BMP085_TEMPDATA));
#endif
    _rawTemperature = I2CUtils::read16(BMP085_I2CADDR, BMP085_TEMPDATA);
}

void BMP085Sensor::readRawPressure(void)
{
    I2CUtils::write8(BMP085_I2CADDR, BMP085_CONTROL, BMP085_READPRESSURECMD + (_samplingMode << 6));
    switch(_samplingMode) {
        case UltraLowPowerMode:
            delay(5);
            break;
        case StandardMode:
            delay(8);
            break;
        case HighResMode:
            delay(14);
            break;
        default:
            delay(26);
            break;
    }

    _rawPressure = I2CUtils::read16(BMP085_I2CADDR, BMP085_PRESSUREDATA);
    _rawPressure <<= 8;
    _rawPressure |= I2CUtils::read8(BMP085_I2CADDR, BMP085_PRESSUREDATA+2);
    _rawPressure >>= (8 - _samplingMode);

 /* this pull broke stuff, look at it later?
    if (_samplingMode==0) {
        _rawPressure <<= 8;
        _rawPressure |= I2CUtils::read8(BMP085_I2CADDR, BMP085_PRESSUREDATA+2);
        _rawPressure >>= (8 - _samplingMode);
    }
 */

#if BMP085_DEBUG == 1
    Serial.print("Raw pressure: "); Serial.println(_rawPressure);
#endif
}

boolean BMP085Sensor::printAddress(Stream *serial)
{
    serial->print(BMP085_I2CADDR, HEX);
}

boolean BMP085Sensor::printValues(Stream *serial)
{
    serial->print(this->temperature());
    serial->print(" ");
    serial->print(this->pressure());
}

int32_t BMP085Sensor::pressure(void)
{
    int32_t UP, B3, B5, B6, X1, X2, X3, p;
    uint32_t B4, B7;

#if BMP085_DEBUG == 1
    // use datasheet numbers!
    _rawTemperature = 27898;
    _rawPressure = 23843;
    ac6 = 23153;
    ac5 = 32757;
    mc = -8711;
    md = 2868;
    b1 = 6190;
    b2 = 4;
    ac3 = -14383;
    ac2 = -72;
    ac1 = 408;
    ac4 = 32741;
    _samplingMode = 0;
#endif

    // do temperature calculations
    X1=(_rawTemperature-(int32_t)(ac6))*((int32_t)(ac5))/pow(2,15);
    X2=((int32_t)mc*pow(2,11))/(X1+(int32_t)md);
    B5=X1 + X2;

#if BMP085_DEBUG == 1
    Serial.print("X1 = "); Serial.println(X1);
    Serial.print("X2 = "); Serial.println(X2);
    Serial.print("B5 = "); Serial.println(B5);
#endif

    // do pressure calcs
    B6 = B5 - 4000;
    X1 = ((int32_t)b2 * ( (B6 * B6)>>12 )) >> 11;
    X2 = ((int32_t)ac2 * B6) >> 11;
    X3 = X1 + X2;
    B3 = ((((int32_t)ac1*4 + X3) << _samplingMode) + 2) / 4;

#if BMP085_DEBUG == 1
    Serial.print("B6 = "); Serial.println(B6);
    Serial.print("X1 = "); Serial.println(X1);
    Serial.print("X2 = "); Serial.println(X2);
    Serial.print("B3 = "); Serial.println(B3);
#endif

    X1 = ((int32_t)ac3 * B6) >> 13;
    X2 = ((int32_t)b1 * ((B6 * B6) >> 12)) >> 16;
    X3 = ((X1 + X2) + 2) >> 2;
    B4 = ((uint32_t)ac4 * (uint32_t)(X3 + 32768)) >> 15;
    B7 = ((uint32_t)_rawPressure - B3) * (uint32_t)( 50000UL >> _samplingMode );

#if BMP085_DEBUG == 1
    Serial.print("X1 = "); Serial.println(X1);
    Serial.print("X2 = "); Serial.println(X2);
    Serial.print("B4 = "); Serial.println(B4);
    Serial.print("B7 = "); Serial.println(B7);
#endif

    if (B7 < 0x80000000) {
        p = (B7 * 2) / B4;
    } else {
        p = (B7 / B4) * 2;
    }
    X1 = (p >> 8) * (p >> 8);
    X1 = (X1 * 3038) >> 16;
    X2 = (-7357 * p) >> 16;

#if BMP085_DEBUG == 1
    Serial.print("p = "); Serial.println(p);
    Serial.print("X1 = "); Serial.println(X1);
    Serial.print("X2 = "); Serial.println(X2);
#endif

    p = p + ((X1 + X2 + (int32_t)3791)>>4);
#if BMP085_DEBUG == 1
    Serial.print("p = "); Serial.println(p);
#endif
    return p;
}

float BMP085Sensor::temperature(void)
{
    int32_t X1, X2, B5;     // following ds convention
    float temp;

#if BMP085_DEBUG == 1
    // use datasheet numbers!
    _rawTemperature = 27898;
    ac6 = 23153;
    ac5 = 32757;
    mc = -8711;
    md = 2868;
#endif

    // step 1
    X1 = (_rawTemperature - (int32_t)ac6) * ((int32_t)ac5) / pow(2,15);
    X2 = ((int32_t)mc * pow(2,11)) / (X1+(int32_t)md);
    B5 = X1 + X2;
    temp = (B5+8)/pow(2,4);
    temp /= 10;
  
    return temp;
}
