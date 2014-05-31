#include <RF12PayloadStream.h>

RF12PayloadStream::RF12PayloadStream(void)
{
    this->flush();
}

size_t RF12PayloadStream::write(uint8_t c)
{
    if (_size < RF12_MAXDATA) {
        _payload[_size] = c;
        _size++;
        _payload[_size] = 0;
        return 1;
    } else {
        return 0;
    }
}
