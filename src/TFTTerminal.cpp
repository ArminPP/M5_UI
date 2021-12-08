#include <Arduino.h>
#include <M5Stack.h>

#include "TFTTerminal.h"

void printTerminal(TFT_eSprite &Terminal, MsgType mt, const char *msg)
{
  char msgT[4] = {'\0'};             // default
  uint16_t msgTypeColor = TFT_GREEN; // default

  Terminal.setTextColor(TERMINAL_TEXT_COLOR, TERMINAL_BGRND_COLOR);
  Terminal.setTextSize(1);
  Terminal.scroll(0, 10);

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
  Terminal.setCursor(5, 2);
  Terminal.printf("00:%02i", (millis() / 1000) % 100);

  Terminal.setTextColor(msgTypeColor, TERMINAL_BGRND_COLOR);
  Terminal.setCursor(40, 2);
  Terminal.printf("%s", msgT);

  Terminal.setTextColor(TERMINAL_TEXT_COLOR, TERMINAL_BGRND_COLOR);
  Terminal.setCursor(65, 2);
  Terminal.printf("%s", msg);

}

void setupTerminal(TFT_eSprite &Terminal)
{
  Terminal.setColorDepth(4);                              // max 16 graph lines with different colors
  Terminal.createSprite(TERMINAL_WIDTH, TERMINAL_HEIGTH); // height = width at M5Stack (landscape mode!)

  Terminal.fillSprite(TERMINAL_BGRND_COLOR);                                           // Note: Sprite is filled with black when created
  Terminal.setScrollRect(0, 0, TERMINAL_WIDTH, TERMINAL_HEIGTH, TERMINAL_BGRND_COLOR); // INFO: to choose a different sroll color than black/white it is mandatory to set a scrollRect !!!!
}

void showTerminal(TFT_eSprite &Terminal)
{
  Terminal.pushSprite(TERMINAL_LEFT_X, TERMINAL_UPPER_Y); // left upper position
}