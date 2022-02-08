#ifndef LOG_h
#define LOG_h

// 'printf' -like function to print different devices, like TFT, serial and SD-card ....
// Returns the number of chars printed, or a negative number in the event of an error.
// Error Return codes:
//     1. INT_MIN if vsnprintf encoding error, OR
//     2. negative of the number of chars it *would have printed* had the buffer been large enough (ie: buffer would
//     have needed to be the absolute value of this size + 1 for null terminator)
//
// Found at https://stackoverflow.com/a/54916870

#define _header_ "File:%s Line:%d Function:%s() %s",__FILE__,__LINE__,__FUNCTION__, //__func__,  // pretty header e.g. for debugging issues

#include <Arduino.h>
#include "Credentials.h"

int LOG(Credentials::LogMsgType mt, const char *format, ...); // max length of message should be 35 chars (because of TFT)!

#endif // LOG_h