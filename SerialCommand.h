#ifndef SerialCommand_h
#define SerialCommand_h

#include <Arduino.h>

class SerialCommand
{
protected:
    Stream              *_stream;
    char                _command[255];
    unsigned int        _commandLength;
    boolean             _hasCommand;

public:
    SerialCommand(Stream *stream = NULL);
    
    void setStream(Stream *stream) { _stream = stream; };
    boolean loop(void);
    const char *getCommand(void) { return _command; };
    void purgeCommand(void) { _command[0] = 0; _commandLength = 0; _hasCommand = false; };
};

#endif // SerialCommand_h