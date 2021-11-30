#include <Arduino.h>
#include <UI.h>
#include <M5Stack.h>

// NO FREE FONTS! Flicker when updating :-(
// #include "Free_Fonts.h" // Include the header file attached to this sketch

// global Variables
int8_t showScreen = 0; // Define the variable: default display mode 0=HOME 1=GRAPH 2=SETUP

// internal Variables
uint8_t timerLCD = 0;

void UI_deleteCanvas()
{
  M5.Lcd.fillRect(CANVAS_X, CANVAS_Y, CANVAS_WIDTH, CANVAS_HEIGHT, CANVAS_BACKGROUND); // x0,y0 are at top left
  // M5.Lcd.drawRect(CANVAS_X, CANVAS_Y, CANVAS_WIDTH, CANVAS_HEIGHT, CANVAS_FRAME_COLOR);
  M5.Lcd.drawRoundRect(CANVAS_X, CANVAS_Y, CANVAS_WIDTH, CANVAS_HEIGHT, 6, CANVAS_FRAME_COLOR);
}

void UI_drawHeader(const char *Title, bool WiFi, bool LAN, bool AP, bool CLOCK, bool BATTERY)
{
  M5.Lcd.fillRect(0, 0, SCREEN_WIDTH, HEADER_HEIGHT, HEADER_BACKGROUND);

  M5.Lcd.setTextColor(HEADER_TEXT_COLOR, HEADER_BACKGROUND);
  M5.Lcd.setTextSize(2);
  M5.Lcd.drawString(Title, HEADER_TEXT_X, HEADER_TEXT_Y);
}

// void UI_drawFooter(const char *Btn1, const char *Btn2, const char *Btn3, bool btn1, bool btn2, bool btn3)
void UI_drawFooter(const char *Btn1, const char *Btn2, const char *Btn3)
{
  M5.Lcd.setTextSize(2);
  // M5.Lcd.setFreeFont(FSS9);
  M5.Lcd.setTextColor(BUTTON_TEXT_COLOR, BUTTON_COLOR);

  // paint Buttons
  if (strlen(Btn1) > 0)
  {
    M5.Lcd.fillRoundRect(BUTTON_1_X, BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT, 6, BUTTON_COLOR);
    M5.Lcd.drawString(Btn1, BUTTON_1_TEXT_X, BUTTON_TEXT_Y);
  }
  else
  {
    // delete Button
    M5.Lcd.fillRoundRect(BUTTON_1_X, BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT, 6, FOOTER_BACKGROUND);
  }
  if (strlen(Btn2) > 0)
  {
    M5.Lcd.fillRoundRect(BUTTON_2_X, BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT, 6, BUTTON_COLOR);
    M5.Lcd.drawString(Btn2, BUTTON_2_TEXT_X, BUTTON_TEXT_Y);
  }
  else
  {
    // delete Button
    M5.Lcd.fillRoundRect(BUTTON_2_X, BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT, 6, FOOTER_BACKGROUND);
  }
  if (strlen(Btn3) > 0)
  {
    M5.Lcd.fillRoundRect(BUTTON_3_X, BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT, 6, BUTTON_COLOR);
    M5.Lcd.drawString(Btn3, BUTTON_3_TEXT_X, BUTTON_TEXT_Y);
  }
  else
  {
    // delete Button
    M5.Lcd.fillRoundRect(BUTTON_3_X, BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT, 6, FOOTER_BACKGROUND);
  }
}

void UI_drawMenue(int8_t activeItem)
{
  M5.Lcd.setTextSize(1);
  int16_t space = (TFT_HEIGHT / noOfScreens); //- 5
  for (int i = 0; i < noOfScreens; ++i)
  {
    if (i == activeItem)
    { // 120
      M5.Lcd.fillRoundRect((i * space), HEADER_HEIGHT, space, MENU_HEIGHT + 5, 0, MENU_COLOR);
      M5.Lcd.setTextColor(MENU_TEXT_COLOR, MENU_COLOR); // if text changes (eg. temperature value) explicit add canvas color as background color
      M5.Lcd.drawString(screenName[i], i * space + MENU_TEXT_X, HEADER_HEIGHT + 12);
    }
    else
    {
      M5.Lcd.fillRoundRect((i * space), HEADER_HEIGHT, space, MENU_HEIGHT, 0, MENU_INACTIVE_COLOR);l
      M5.Lcd.setTextColor(MENU_TEXT_COLOR, MENU_INACTIVE_COLOR); // if text changes (eg. temperature value) explicit add canvas color as background color
      M5.Lcd.drawString(screenName[i], i * space + MENU_TEXT_X, HEADER_HEIGHT + 12);
    }
  }
}

//  ------------------------------------------------------------------
//  --------------------------- SCREENS ------------------------------
//  ------------------------------------------------------------------
void UI_showHome()
{
  M5.Lcd.setTextColor(CANVAS_TEXT_COLOR, CANVAS_BACKGROUND); // Bei sich ändernden Texten Hintergrund mitangeben!!!!
  M5.Lcd.setTextSize(2);
  // M5.Lcd.fillRect(CANVAS_X, CANVAS_Y, CANVAS_HEIGHT, CANVAS_WIDTH, CANVAS_BACKGROUND);
  M5.Lcd.drawString("Temperature 1: " + String((millis() % 128) * 0.1) + " C  ", 10, HEADER_HEIGHT + 30 + 10);
  M5.Lcd.drawString("Temperature 2: " + String((millis() % 118) * 0.1) + " C  ", 10, HEADER_HEIGHT + 30 + 30);
  M5.Lcd.drawString("Temperature 3: " + String((millis() % 108) * 0.1) + " C  ", 10, HEADER_HEIGHT + 30 + 50);
  M5.Lcd.drawString("Humidity:      " + String((millis() % 102)) + " %   ", 10, HEADER_HEIGHT + 30 + 70);
}

void UI_showGraph()
{
  M5.Lcd.setCursor(30, 110);
  M5.Lcd.setTextColor(CANVAS_TEXT_COLOR, CANVAS_BACKGROUND); // Bei sich ändernden Texten Hintergrund mitangeben!!!!
  M5.Lcd.setTextSize(3);
  M5.Lcd.print("Config ");
  M5.Lcd.println(millis());
}

void UI_showSetup()
{
  M5.Lcd.setCursor(30, 110);
  M5.Lcd.setTextColor(CANVAS_TEXT_COLOR, CANVAS_BACKGROUND); // Bei sich ändernden Texten Hintergrund mitangeben!!!!
  M5.Lcd.setTextSize(3);
  M5.Lcd.print("SETUP ");
  M5.Lcd.println(millis());
}

void UI_showScreen4()
{
  M5.Lcd.setCursor(30, 110);
  M5.Lcd.setTextColor(CANVAS_TEXT_COLOR, CANVAS_BACKGROUND); // Bei sich ändernden Texten Hintergrund mitangeben!!!!
  M5.Lcd.setTextSize(3);
  M5.Lcd.print("Screen 4");
}

void UI_showScreen5()
{
  M5.Lcd.setCursor(30, 110);
  M5.Lcd.setTextColor(CANVAS_TEXT_COLOR, CANVAS_BACKGROUND); // Bei sich ändernden Texten Hintergrund mitangeben!!!!
  M5.Lcd.setTextSize(3);
  M5.Lcd.print("Screen 5");
}
//  ------------------------------------------------------------------
//  ------------------------------------------------------------------
//  ------------------------------------------------------------------

void UI_restartTimerLCD()
{
  timerLCD = 0;
  UI_drawHeader(HEADER_TITLE, 0, 0, 0, 0, 0); // repaint header, to clear embedded progress bar !!
  UI_showTimeoutProgressLCD(0, LCD_TIMEOUT);
  M5.Lcd.setBrightness(LCD_BRIGHTNESS); // set default brightness
}

void UI_showTimeoutProgressLCD(int progress, int max)
{
  int i;
  if ((progress == 0) || (max == 0))
    i = 0; // div zero
  else
  {
    i = 100 / max;    // int division (result is int)
    i = i * progress; // scale progress to 100%
  }
  M5.Lcd.progressBar(0, HEADER_HEIGHT - 4, M5.Lcd.width(), 4, i);

  // void M5Display::progressBar(int x, int y, int w, int h, uint8_t val) {
  //  // drawRect(x, y, w, h, 0x09F1);   // FIX: <------------------------          remove rectangle!
  //   fillRect(x + 1, y + 1, w * (((float)val) / 100.0), h - 1, 0x09F1);
  // }
}

void UI_timeoutLCD()
{
  // After timeout (e.g. 30 s) display will get dark
  //
  // update screen every 'refresh' ms
  static unsigned long timeoutPM = 0;
  unsigned long timeoutCM = millis();
  if (timeoutCM - timeoutPM >= 1000)
  {
    timeoutPM = timeoutCM;

    if (timerLCD <= LCD_TIMEOUT) // because of overflow of int_8
    {
      timerLCD += 1;
      UI_showTimeoutProgressLCD(timerLCD, LCD_TIMEOUT);
    }

    // LCD brightnes fade out...
    // Brightness (0: Off - 255: Full)
    if (timerLCD == LCD_TIMEOUT - 5)
    {
      M5.Lcd.setBrightness(80);
    }
    if (timerLCD == LCD_TIMEOUT - 4)
    {
      M5.Lcd.setBrightness(70);
    }
    if (timerLCD == LCD_TIMEOUT - 3)
    {
      M5.Lcd.setBrightness(60);
    }
    if (timerLCD == LCD_TIMEOUT - 2)
    {
      M5.Lcd.setBrightness(30);
    }
    if (timerLCD == LCD_TIMEOUT - 1)
    {
      M5.Lcd.setBrightness(20);
    }
    if (timerLCD == LCD_TIMEOUT)
    {
      M5.Lcd.setBrightness(0);
    }
  }
}
void UI_showActiveScreen(uint8_t screen)
{
  // DON'T delete canvas in here
  // because in 1s refreshing mode
  // there will be flickering!
  //
  switch (screen)
  {
  case HOME:
    UI_showHome();
    break;
  case GRAPH:
    UI_showGraph();
    break;
  case SETUP:
    UI_showSetup();
    break;
  case SCREEN4:
    UI_showScreen4();
    break;
  case SCREEN5:
    UI_showScreen5();
    break;
  }
}

void UI_doHandleTFT(int16_t refresh)
{
  if (M5.BtnA.wasReleased())
  {
    showScreen--;
    if (showScreen <= 0)
    {
      showScreen = 0;
      UI_drawFooter("", "", ">");
      UI_drawMenue(showScreen);
    }
    else
    {
      UI_drawFooter("<", "", ">");
      UI_drawMenue(showScreen);
    }
    UI_deleteCanvas();               // delete Canvas
    UI_showActiveScreen(showScreen); // show screen immedeatly
    UI_restartTimerLCD();
  }
  else if (M5.BtnB.wasReleased())
  {
    UI_restartTimerLCD();
  }
  else if (M5.BtnC.wasReleased())
  {
    showScreen++;
    Serial.print(showScreen);
    if (showScreen >= noOfScreens - 1)
    {
      showScreen = noOfScreens - 1; // eg. 0..4 == 5-1
      UI_drawFooter("<", "", "");
      UI_drawMenue(showScreen);
    }
    else
    {
      UI_drawFooter("<", "", ">");
      UI_drawMenue(showScreen);
    }

    UI_deleteCanvas();               // NEW delete Canvas
    UI_showActiveScreen(showScreen); // show screen immedeatly
    UI_restartTimerLCD();
  }

  UI_timeoutLCD(); // manage timeout for LCD brightness

  // update screen every 'refresh' ms
  static unsigned long refreshLcdPM = 0;
  unsigned long refreshLcdCM = millis();
  if (refreshLcdCM - refreshLcdPM >= refresh)
  {
    refreshLcdPM = refreshLcdCM;
    UI_showActiveScreen(showScreen);
  }
}
