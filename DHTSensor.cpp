/* DHT library 

MIT license
written by Adafruit Industries
*/

#include "DHTSensor.h"

#define DELAY_BETWEEN_READ 2000

DHTSensor::DHTSensor(uint8_t pin, DHTSensorType type)
{
    _pin = pin;
    _type = type;
}

const char *DHTSensor::sensorClass(void)
{
    return "DHT";
}

const char *DHTSensor::sensorType(void)
{
    switch(_type) {
        case DHTSensorType_11:
            return "DHT11";
        case DHTSensorType_21:
            return "DHT21";
        case DHTSensorType_22:
            return "DHT22";
        case DHTSensorType_AM2301:
            return "AM2301";
    }
}

boolean DHTSensor::begin(void)
{
    // set up the pins!
    pinMode(_pin, INPUT);
    digitalWrite(_pin, HIGH);
    _lastreadtime = 0;
    return true;
}

boolean DHTSensor::printValues(Stream *serial)
{
    serial->print(this->temperature());
    serial->print(" ");
    serial->print(this->humidity());
    return true;
}

boolean DHTSensor::loop(void)
{
    uint8_t data[6];
    uint8_t laststate = HIGH;
    uint8_t counter = 0;
    uint8_t j = 0, i;
    unsigned long currentMillis = millis();

    if ((unsigned long)(currentMillis - _lastreadtime) < DELAY_BETWEEN_READ) {
        return true; // return last correct measurement
    }

    _lastreadtime = currentMillis;
    // pull the pin high and wait 250 milliseconds
    digitalWrite(_pin, HIGH);
    delay(250);

    data[0] = data[1] = data[2] = data[3] = data[4] = 0;
  
    // now pull it low for ~20 milliseconds
    pinMode(_pin, OUTPUT);
    digitalWrite(_pin, LOW);
    delay(20);

    noInterrupts();
    digitalWrite(_pin, HIGH);
    delayMicroseconds(40);
    pinMode(_pin, INPUT);

    // read in timings
    for ( i=0; i< MAXTIMINGS; i++) {
        counter = 0;
        while (digitalRead(_pin) == laststate) {
            counter++;
            delayMicroseconds(1);
            if (counter == 255) {
                break;
            }
        }
        laststate = digitalRead(_pin);

        if (counter == 255) break;

        // ignore first 3 transitions
        if ((i >= 4) && (i%2 == 0)) {
            // shove each bit into the storage bytes
            data[j/8] <<= 1;
            if (counter > 6)
            data[j/8] |= 1;
            j++;
        }
    }
    interrupts();


    /*
    Serial.println(j, DEC);
    Serial.print(data[0], HEX); Serial.print(", ");
    Serial.print(data[1], HEX); Serial.print(", ");
    Serial.print(data[2], HEX); Serial.print(", ");
    Serial.print(data[3], HEX); Serial.print(", ");
    Serial.print(data[4], HEX); Serial.print(" =? ");
    Serial.println(data[0] + data[1] + data[2] + data[3], HEX);
    */

    // check we read 40 bits and that the checksum matches
    if ((j >= 40) &&  (data[4] == ((data[0] + data[1] + data[2] + data[3]) & 0xFF)) ) {
        switch (_type) {
            case DHTSensorType_11:
                _humidity = data[0];
                break;
            case DHTSensorType_22:
            case DHTSensorType_21:
            case DHTSensorType_AM2301:
                _humidity = data[0];
                _humidity *= 256;
                _humidity += data[1];
                _humidity /= 10;
                break;
        }
        switch (_type) {
            case DHTSensorType_11:
                _temperature = data[2];
                break;
            case DHTSensorType_22:
            case DHTSensorType_21:
            case DHTSensorType_AM2301:
                _temperature = data[2] & 0x7F;
                _temperature *= 256;
                _temperature += data[3];
                _temperature /= 10;
                if (data[2] & 0x80)
                _temperature *= -1;
                break;
        }
        return true;
    }
  
    return false;
}
