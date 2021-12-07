#include <Arduino.h>
#include <M5Stack.h>

#include "TFTTerminal.h"

void TerminalPrint(TFT_eSprite &terminal, MsgType mt, const char *msg)
{
  char msgT[4] = {'\0'};             // default
  uint16_t msgTypeColor = TFT_GREEN; // default

  terminal.setTextColor(TERMINAL_TEXT_COLOR, TERMINAL_BGRND_COLOR);
  terminal.setTextSize(1);
  terminal.scroll(0, 10);

  switch (mt)
  {
  case NONE:
    // use default values!
    break;
  case INFO:
    snprintf(msgT, sizeof(msgT), "[%c]", 'I');
    msgTypeColor = TFT_BLUE;
    break;
  case WARNING:
    snprintf(msgT, sizeof(msgT), "[%c]", 'W');
    msgTypeColor = TFT_YELLOW;
    break;
  case ERROR:
    snprintf(msgT, sizeof(msgT), "[%c]", 'E');
    msgTypeColor = TFT_RED;
    break;
  }
  terminal.setCursor(5, 2);
  terminal.printf("00:%02i", (millis() / 1000) % 100);

  terminal.setTextColor(msgTypeColor, TERMINAL_BGRND_COLOR);
  terminal.setCursor(40, 2);
  terminal.printf("%s", msgT);

  terminal.setTextColor(TERMINAL_TEXT_COLOR, TERMINAL_BGRND_COLOR);
  terminal.setCursor(65, 2);
  terminal.printf("%s", msg);

  terminal.pushSprite(TERMINAL_LEFT_X, TERMINAL_UPPER_Y); // left upper position
}