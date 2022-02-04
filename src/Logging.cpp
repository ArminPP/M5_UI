#include <Arduino.h>
#include <M5Stack.h>

#include "Logging.h""


#define CREDENTIALS_LOG_DEBUG 1
#define CREDENTIALS_LOG_ERROR 1
#define CREDENTIALS_LOG_WARNING 1
#define CREDENTIALS_LOG_INFO 1



int LOG(LogMsgType mt, const char * format, ...)
// `printf` -like function to print to the LCD display.
// Returns the number of chars printed, or a negative number in the event of an error.
// Error Return codes:
//     1. INT_MIN if vsnprintf encoding error, OR
//     2. negative of the number of chars it *would have printed* had the buffer been large enough (ie: buffer would
//     have needed to be the absolute value of this size + 1 for null terminator)
// 
// Found at https://stackoverflow.com/a/54916870

{
  int return_code;

  // Formatted string buffer: make as long as you need it to be to hold the longest string you'd ever want
  // to print + null terminator
  char formatted_str[128];

  va_list arglist;
  va_start(arglist, format);

  // Produce the formatted string; see vsnprintf documentation: http://www.cplusplus.com/reference/cstdio/vsnprintf/
  int num_chars_to_print = vsnprintf(formatted_str, sizeof(formatted_str), format, arglist);
  va_end(arglist);

  if (num_chars_to_print < 0)
  {
    // Encoding error
    return_code = INT_MIN;
    return return_code; // exit early
  }
  else if (num_chars_to_print >= sizeof(formatted_str))
  {
    // formatted_str buffer not long enough
    return_code = -num_chars_to_print;
    // Do NOT return here; rather, continue and print what we can
  }
  else
  {
    // No error
    return_code = num_chars_to_print;
  }

// Now do whatever is required to send the formatted_str buffer to the LCD display here.
if (CREDENTIALS_LOG_ERROR && (mt==LOG_ERROR)){
  Serial.printf("%s [%s] %s", "04-02-2022|10:30", msg[mt], formatted_str);
}
else if (CREDENTIALS_LOG_INFO && (mt==LOG_INFO)){
  Serial.printf("%s [%s] %s", " 04-02-2022|10:30", msg[mt], formatted_str);
}
else if (mt==LOG_NONE){
  Serial.printf("%s [%s] %s", "  04-02-2022|10:30", msg[mt], formatted_str);
}



  return return_code;
}