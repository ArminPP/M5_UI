#ifndef TFTTERMINAL_h
#define TFTTERMINAL_h

/*
Simple TFT Terminal, with *no* buffer.
It only shows the actual events/lines - no scrolling back!
Lines are formatted like: 04-02-2022|10:30 [I] 97.231.509 1234567890 SuperINFO

Armin (c) 2022
*/

#include <Arduino.h>
#include "Credentials.h"

// https://www.barth-dev.de/online/rgb565-color-picker/


void printTerminal(TFT_eSprite &Terminal,  const char *dt, Credentials::LogMsgType mt, const char *msg);

void setupTerminal(TFT_eSprite &Terminal);
void showTerminal(TFT_eSprite &Terminal);

#endif // TFTTERMINAL_h