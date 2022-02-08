#ifndef CREDENTIALS_h
#define CREDENTIALS_h

#include <Arduino.h>
#include <M5Display.h>
#include "Credentials.h"

namespace Credentials // https://stackoverflow.com/questions/2483978/best-way-to-implement-globally-scoped-data
{
  // --- Global ---

  bool LOG_TO_SERIAL = true;
  bool LOG_TO_TFT = true;
  bool LOG_TO_FILE = false;

  bool ENABLE_LOG_DEBUG = true;
  bool ENABLE_LOG_ERROR = true;
  bool ENABLE_LOG_WARNING = true;
  bool ENABLE_LOG_INFO = true;

  const char *SERIAL_TERMINAL_RED = "\e[1;31m"; // have not figured out now, which terminal is able to show colored chars...
  const char *SERIAL_TERMINAL_LL_GREEN = "\e[32m";
  const char *SERIAL_TERMINAL_LL_YELLOW = "\e[1;33m";
  const char *SERIAL_TERMINAL_LL_NORM = "\e[0m";

  // https://www.barth-dev.de/online/rgb565-color-picker/
  uint16_t TERMINAL_BGRND_COLOR = 0x10A2; // TFT_BLACK //
  uint16_t TERMINAL_TEXT_COLOR = TFT_GREEN;
  uint16_t TERMINAL_INFO_COLOR = TFT_BLUE;
  uint16_t TERMINAL_WARNING_COLOR = TFT_YELLOW;
  uint16_t TERMINAL_ERROR_COLOR = TFT_RED;
  uint16_t TERMINAL_DEBUG_COLOR = TFT_WHITE;

  uint16_t TFT_REFRESH_RATE = 3000; // screen refreshrate in ms
  uint16_t TFT_TIMEOUT = 300;       // 300 sec - timeout for backlit
  uint8_t TFT_BRIGHTNESS = 250;     // 0 .. 255  -->  default value, after timeout it is zero!

  enum LogMsgType
  {
    LOG_NONE,
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR,
    LOG_DEBUG
  };
  const char *LOG_MSG[5] = {"N", "I", "W", "E", "D"};

}

#endif // CREDENTIALS_h