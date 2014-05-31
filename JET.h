#ifndef JET_h
#define JET_h

#include <Arduino.h>

#define SERIAL_SPEED        115200
#define NODE_BEGIN_DELAY    2000


// set the sync mode to 2 if the fuses are still the Arduino default
// mode 3 (full powerdown) can only be used with 258 CK startup fuses
#define RADIO_SYNC_MODE 2

void transfertRF12DataToStream(Stream* stream);

#endif // JET_h
