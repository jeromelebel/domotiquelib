#ifndef RFM12b_h
#define RFM12b_h

#include <arduino.h>

typedef enum {
    RFM12b_FREQUENCY_433 = 1,
    RFM12b_FREQUENCY_868 = 2,
    RFM12b_FREQUENCY_915 = 3
} RFM12b_FREQUENCY;

#define RF_MAX   (RF12_MAXDATA + 5)

class RFM12b
{
protected:
    int _spiPin;
    int _irqPin;
    int _id;
    int _group;
    RFM12b_FREQUENCY _band;

    uint16_t sendCommand(uint16_t cmd);
    void interruption();
    
    friend void rfm12Interrupt();

public:
    RFM12b(int spiPin, int irqPin, RFM12b_FREQUENCY band, int id, int group);
    void init(void);
    
};

#endif // RFM12b_h
