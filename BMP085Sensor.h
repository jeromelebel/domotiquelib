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

#include <Wire.h>
#include <Arduino.h>
#include <Sensor.h>

class BMP085Sensor : public Sensor {
public:
    typedef enum {
        UltraLowPowerMode,
        StandardMode,
        HighResMode,
        UltraHighResMode
    } SamplingMode;

private:
    SamplingMode                _samplingMode;
    uint16_t                    _rawTemperature;
    uint32_t                    _rawPressure;

    int16_t ac1, ac2, ac3, b1, b2, mb, mc, md;
    uint16_t ac4, ac5, ac6;
    
    void readRawTemperature(void);
    void readRawPressure(void);
    
public:
    BMP085Sensor(SamplingMode mode = UltraHighResMode);
    const char *sensorClass(void);
    virtual const char *sensorType(void) { return this->sensorClass(); };
    boolean begin(void);
    boolean readValues(void) { this->readRawTemperature(); this->readRawPressure(); }
    boolean printAddress(Stream *serial);
    boolean printValues(Stream *serial);
    float temperature(void);
    int32_t pressure(void);
};
