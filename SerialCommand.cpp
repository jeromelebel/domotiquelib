#include <SerialCommand.h>

SerialCommand::SerialCommand(Stream *stream)
{
    this->setStream(stream);
    this->purgeCommand();
}

boolean SerialCommand::loop(void)
{
    if (!_hasCommand && _stream && _stream->available() > 0) {
        char a;
        
        a = _stream->read();
        if (a == '\n' || a == '\r') {
            _hasCommand = true;
        } else {
            _command[_commandLength] = a;
            _commandLength++;
            _command[_commandLength] = 0;
            _hasCommand = _commandLength == sizeof(_command);
        }
    }
    return _hasCommand;
}
