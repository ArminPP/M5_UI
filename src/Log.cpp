#include <Arduino.h>
#include <M5Stack.h>

#include "Log.h"
#include "Display.h"

int LOG(Credentials::LogMsgType mt, const char *format, ...) // max length of message should be 35 chars (because of TFT)!

// 'printf' -like function to print different devices, like TFT, serial and SD-card ....
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
  char formatted_str[86]; // 2 lines of tft screen

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
  //
  //

  // set here the dynamic icons only!
  if (mt == Credentials::LOG_INFO)
    ico_INFO = true;
  if (mt == Credentials::LOG_WARNING)
    ico_WARN = true;
  if (mt == Credentials::LOG_ERROR)
    ico_ERR = true;

  if (Credentials::LOG_TO_SERIAL)
  {
    if (!((!Credentials::CREDENTIALS_LOG_INFO) && (mt == Credentials::LOG_INFO)) ||      // filter: only if msg and credentials are equal then print msg!
        ((!Credentials::CREDENTIALS_LOG_WARNING) && (mt == Credentials::LOG_WARNING)) || // LOG_NONE also will be printed!
        ((!Credentials::CREDENTIALS_LOG_ERROR) && (mt == Credentials::LOG_ERROR)) ||
        ((!Credentials::CREDENTIALS_LOG_DEBUG) && (mt == Credentials::LOG_DEBUG)))
    {
      Serial.printf("%s [%s] %s\n", "04-02-2022|10:30", Credentials::LOG_MSG[mt], formatted_str); // timestamp, msg type, message
    }
  }
  if (Credentials::LOG_TO_TFT)
  {
    if (!((!Credentials::CREDENTIALS_LOG_INFO) && (mt == Credentials::LOG_INFO)) ||      // filter: only if msg and credentials are equal then print msg!
        ((!Credentials::CREDENTIALS_LOG_WARNING) && (mt == Credentials::LOG_WARNING)) || // LOG_NONE also will be printed!
        ((!Credentials::CREDENTIALS_LOG_ERROR) && (mt == Credentials::LOG_ERROR)) ||
        ((!Credentials::CREDENTIALS_LOG_DEBUG) && (mt == Credentials::LOG_DEBUG)))
    {
      UI_TerminalPrint("10:30", mt, formatted_str); // only time, msg type, message
    }
  }
  if (Credentials::LOG_TO_FILE)
  {
    if (!((!Credentials::CREDENTIALS_LOG_INFO) && (mt == Credentials::LOG_INFO)) ||      // filter: only if msg and credentials are equal then print msg!
        ((!Credentials::CREDENTIALS_LOG_WARNING) && (mt == Credentials::LOG_WARNING)) || // LOG_NONE also will be printed!
        ((!Credentials::CREDENTIALS_LOG_ERROR) && (mt == Credentials::LOG_ERROR)) ||
        ((!Credentials::CREDENTIALS_LOG_DEBUG) && (mt == Credentials::LOG_DEBUG)))
    {
    }
  }

  // finito!
  return return_code;
}