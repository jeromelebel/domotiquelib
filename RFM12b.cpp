#include <RFM12b.h>
#include <SPI.h>

#define RF_RECEIVER_ON  0x82DD
#define RF_XMITTER_ON   0x823D
#define RF_IDLE_MODE    0x820D
#define RF_SLEEP_MODE   0x8205
#define RF_WAKEUP_MODE  0x8207
#define RF_TXREG_WRITE  0xB800
#define RF_RX_FIFO_READ 0xB000
#define RF_WAKEUP_TIMER 0xE000

int interruption = 0;
static RFM12b *current = NULL;

void rfm12Interrupt()
{
    interruption += 1;
    current->interruption();
}

RFM12b::RFM12b(int spiPin, int irqPin, RFM12b_FREQUENCY band, int id, int group)
{
    _spiPin = spiPin;
    _irqPin = irqPin;
    _band = band;
    _id = id;
    _group = group;
    current = this;
}

void RFM12b::init(void)
{
    uint16_t cmd;
    uint16_t result;
    // put your setup code here, to run once:
    
#if defined(__SAM3X8E__)
    SPI.begin(_spiPin);
#else
    SPI.begin();
#endif
    pinMode(_irqPin, INPUT);
    digitalWrite(_irqPin, 1); // pull-up

    this->sendCommand(0x0000); // intitial SPI transfer added to avoid power-up problem

    this->sendCommand(RF_SLEEP_MODE); // DC (disable clk pin), enable lbd

    // wait until RFM12B is out of power-up reset, this takes several *seconds*
    this->sendCommand(RF_TXREG_WRITE); // in case we're still in OOK mode
    Serial.println("1");
    while (digitalRead(_irqPin) == 0)
        this->sendCommand(0x0000);
    Serial.println("2");
        
    this->sendCommand(0x80C7 | (_band << 4)); // EL (ena TX), EF (ena RX FIFO), 12.0pF 
    this->sendCommand(0xA640); // 868MHz 
    this->sendCommand(0x8239);  
    this->sendCommand(0xC606); // approx 49.2 Kbps, i.e. 10000/29/(1+6) Kbps
    this->sendCommand(0x94A2); // VDI,FAST,134kHz,0dBm,-91dBm 
    this->sendCommand(0xC2AC); // AL,!ml,DIG,DQD4 
    if (_group != 0) {
        this->sendCommand(0xCA83); // FIFO8,2-SYNC,!ff,DR 
        this->sendCommand(0xCE00 | _group); // SYNC=2DXX； 
    } else {
        this->sendCommand(0xCA8B); // FIFO8,1-SYNC,!ff,DR 
        this->sendCommand(0xCE2D); // SYNC=2D； 
    }
    this->sendCommand(0xC483); // @PWR,NO RSTRIC,!st,!fi,OE,EN 
    this->sendCommand(0x9850); // !mp,90kHz,MAX OUT 
    this->sendCommand(0xCC77); // OB1，OB0, LPX,！ddy，DDIT，BW0 
    this->sendCommand(0xE000); // NOT USE 
    this->sendCommand(0xC800); // NOT USE 
    this->sendCommand(0xC049); // 1.66MHz,3.1V 

    attachInterrupt(0, rfm12Interrupt, LOW);
}

uint16_t RFM12b::sendCommand(uint16_t cmd)
{
    uint16_t result = 0;
#if defined(__SAM3X8E__)
    result = SPI.transfer(_spiPin, cmd >> 8, SPI_CONTINUE) << 8;
    result |= SPI.transfer(_spiPin, cmd | 0xFF);
#else
    result = SPI.transfer(cmd >> 8) << 8;
    result |= SPI.transfer(cmd | 0xFF);
#endif
    return result;
}

void RFM12b::interruption(void)
{
}
