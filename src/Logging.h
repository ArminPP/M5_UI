#ifndef LOGGING_h
#define LOGGING_h

#include <Arduino.h>

enum LogMsgType
{
  LOG_NONE,
  LOG_INFO,
  LOG_WARNING,
  LOG_ERROR,
  LOG_DEBUG
};
const char *msg[5] = {"N", "I", "W", "E", "D"};

int LOG(LogMsgType mt, const char * format, ...);

#endif // LOGGING_h