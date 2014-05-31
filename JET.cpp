#include <JET.h>
#if !defined(__SAM3X8E__)
#include <RF12.h>
#endif

void transfertRF12DataToStream(Stream* stream)
{
#if !defined(__SAM3X8E__)
    if (rf12_recvDone()) {
        if (rf12_crc == 0 && (rf12_hdr & RF12_HDR_CTL) == 0) {
            byte n = rf12_len;

            if (n > 0) {
                stream->print((unsigned char)rf12_hdr);
                stream->print(" RFM12B  ");
                for (byte i = 0; i < n; ++i) {
                    unsigned char value = rf12_data[i];
                    
                    if (value < 0x10) stream->print("0");
                    stream->print((unsigned char)value, HEX);
                }
                stream->println();
                if (RF12_WANTS_ACK) {
                    rf12_sendStart(RF12_ACK_REPLY, 0, 0);
                }
            }
        }
    }
#endif
}
