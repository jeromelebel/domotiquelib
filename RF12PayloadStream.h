#ifndef RF12PayloadStream_h
#define RF12PayloadStream_h

#include <arduino.h>
#if !defined(__SAM3X8E__)
#include <RF12.h>
#else
#define RF12_MAXDATA 66
#endif

class RF12PayloadStream : public Stream
{
    unsigned char _payload[RF12_MAXDATA + 1];
    size_t _size;

public:
    RF12PayloadStream(void);
    
    int available() { return 0; };
    int read() { return 0; };
    int peek() { return 0; };
    void flush() { _size = 0; _payload[0] = 0; };
    size_t write(uint8_t c);
    unsigned char *getPayload(void) { return _payload; };
    size_t getSize(void) { return _size; };
};
#endif // RF12PayloadStream_h
