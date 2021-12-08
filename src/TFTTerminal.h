#ifndef TFTTERMINAL_h
#define TFTTERMINAL_h

#include <Arduino.h>

// https://www.barth-dev.de/online/rgb565-color-picker/

// terminal definitions
#define TERMINAL_LEFT_X 15
#define TERMINAL_UPPER_Y 65 // this is the "real" position of the graph
#define TERMINAL_WIDTH 290
#define TERMINAL_HEIGTH 130

#define TERMINAL_BGRND_COLOR 0x10A2 // TFT_BLACK //
#define TERMINAL_TEXT_COLOR TFT_GREEN
#define TERMINAL_INFO_COLOR TFT_BLUE
#define TERMINAL_WARNING_COLOR TFT_YELLOW
#define TERMINAL_ERROR_COLOR TFT_RED

enum MsgType
{
    NONE,
    INFO,
    WARNING,
    ERROR,
};

void printTerminal(TFT_eSprite &Terminal, MsgType mt, const char *msg);

void setupTerminal(TFT_eSprite &Terminal);
void showTerminal(TFT_eSprite &Terminal);

#endif // TFTTERMINAL_h