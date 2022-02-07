#include <Arduino.h>
#include <M5Stack.h>

#include "TFTTerminal.h"

/*
Simple TFT Terminal, with *no* buffer.
It only shows the actual events/lines - no scrolling back!
Lines are formatted like: 04-02-2022|10:30 [I] 97.231.509 1234567890 SuperINFO

Armin (c) 2022
*/

// terminal definitions
#define TERMINAL_LEFT_X 15
#define TERMINAL_UPPER_Y 65 // this is the "real" position of the graph
#define TERMINAL_WIDTH 290
#define TERMINAL_HEIGTH 130

void printTerminal(TFT_eSprite &Terminal, const char *dt, Credentials::LogMsgType mt, const char *msg)
// the terminal prints every line/value in background,
// independend of Credentials::TFT_REFRESH_RATE.
// After a TFT refresh, all lines will be showed.
{
  char msgT[4] = {'\0'};             // default
  uint16_t msgTypeColor = TFT_GREEN; // default

  Terminal.setTextColor(Credentials::TERMINAL_TEXT_COLOR, Credentials::TERMINAL_BGRND_COLOR);
  Terminal.setTextSize(1);

  // handle long lines                           max chars of 1 line:  37 + 11 chars for msg + time and msgtype
  if (strlen(msg) > 37 && (strlen(msg) < 86)) // max chars of 2 lines: 86 (char formatted_str[86] in Log.cpp)
    Terminal.scroll(0, 20);                   // scroll 2 lines
  else if (strlen(msg) >= 86)                 //
    Terminal.scroll(0, 30);                   // scroll 3 lines (more than 3 lines ar not allowed!)
  else                                        //
    Terminal.scroll(0, 10);                   // scroll 1 line

  switch (mt)
  {
  case Credentials::LOG_NONE:
    // use default values!
    break;
  case Credentials::LOG_INFO:
    snprintf(msgT, sizeof(msgT), "[%s]", Credentials::LOG_MSG[mt]);
    msgTypeColor = Credentials::TERMINAL_INFO_COLOR;
    break;
  case Credentials::LOG_WARNING:
    snprintf(msgT, sizeof(msgT), "[%s]", Credentials::LOG_MSG[mt]);
    msgTypeColor = Credentials::TERMINAL_WARNING_COLOR;
    break;
  case Credentials::LOG_ERROR:
    snprintf(msgT, sizeof(msgT), "[%s]", Credentials::LOG_MSG[mt]);
    msgTypeColor = Credentials::TERMINAL_ERROR_COLOR;
    break;
  case Credentials::LOG_DEBUG:
    snprintf(msgT, sizeof(msgT), "[%s]", Credentials::LOG_MSG[mt]);
    msgTypeColor = Credentials::TERMINAL_DEBUG_COLOR;
    break;
  }

  Terminal.setCursor(5, 2);
  Terminal.printf("%s", dt); // date time

  Terminal.setTextColor(msgTypeColor, Credentials::TERMINAL_BGRND_COLOR);
  Terminal.setCursor(40, 2);
  Terminal.printf("%s", msgT); // message type

  Terminal.setTextColor(Credentials::TERMINAL_TEXT_COLOR, Credentials::TERMINAL_BGRND_COLOR);
  Terminal.setCursor(65, 2);
  Terminal.printf("%s", msg); // message
}

void setupTerminal(TFT_eSprite &Terminal)
{
  Terminal.setColorDepth(4);                              // max 16 graph lines with different colors
  Terminal.createSprite(TERMINAL_WIDTH, TERMINAL_HEIGTH); // height = width at M5Stack (landscape mode!)

  Terminal.fillSprite(Credentials::TERMINAL_BGRND_COLOR);                                           // Note: Sprite is filled with black when created
  Terminal.setScrollRect(0, 0, TERMINAL_WIDTH, TERMINAL_HEIGTH, Credentials::TERMINAL_BGRND_COLOR); // INFO: to choose a different sroll color than black/white it is mandatory to set a scrollRect !!!!
}

void showTerminal(TFT_eSprite &Terminal)
{
  Terminal.pushSprite(TERMINAL_LEFT_X, TERMINAL_UPPER_Y); // left upper position
}