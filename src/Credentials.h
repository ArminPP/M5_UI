#ifndef CREDENTIALS_h
#define CREDENTIALS_h

#include <Arduino.h>

namespace Credentials // https://stackoverflow.com/questions/2483978/best-way-to-implement-globally-scoped-data
{
    // --- Global ---

    extern bool LOG_TO_SERIAL;
    extern bool LOG_TO_TFT;
    extern bool LOG_TO_FILE;

    extern bool CREDENTIALS_LOG_DEBUG;
    extern bool CREDENTIALS_LOG_ERROR;
    extern bool CREDENTIALS_LOG_WARNING;
    extern bool CREDENTIALS_LOG_INFO;

    extern const char *SERIAL_TERMINAL_RED[];
    extern const char *SERIAL_TERMINAL_LL_GREEN[];
    extern const char *SERIAL_TERMINAL_LL_YELLOW[];
    extern const char *SERIAL_TERMINAL_LL_NORM[];

    extern uint16_t TERMINAL_BGRND_COLOR;
    extern uint16_t TERMINAL_TEXT_COLOR;
    extern uint16_t TERMINAL_INFO_COLOR;
    extern uint16_t TERMINAL_WARNING_COLOR;
    extern uint16_t TERMINAL_ERROR_COLOR;
    extern uint16_t TERMINAL_DEBUG_COLOR;

    extern uint16_t TFT_REFRESH_RATE; // screen refreshrate in ms
    extern uint16_t TFT_TIMEOUT;      // 300 sec
    extern uint8_t TFT_BRIGHTNESS;    // 0 .. 255  -->  default value, after timeout it is zero!

    enum LogMsgType
    {
        LOG_NONE,
        LOG_INFO,
        LOG_WARNING,
        LOG_ERROR,
        LOG_DEBUG
    };
    extern const char *LOG_MSG[5];

}

#endif // CREDENTIALS_h