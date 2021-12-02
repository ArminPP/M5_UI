#include <Arduino.h>
#include <UI.h>
#include <M5Stack.h>

#include "icons.h"
// NO FREE FONTS! Flicker when updating :-(
// #include "Free_Fonts.h" // Include the header file attached to this sketch

// global Variables
int8_t showScreen = 0; // Define the variable: default display mode 0=HOME 1=ENV 2=ENV_GRAPH

// internal Variables
int16_t timerLCD = 0;

void UI_drawIcons(bool ETH, bool WiFi, bool AP, bool Info, bool Warning, bool Error, bool Clock)
// ETH, WiFi, AP are static icons, the change only if the connection changes
// 'Info, Warning and Error are dynamic icons, if they are triggered they will blink every second
// Clock updates every second... (maybe this ist to fast?)
//

{
  if (ETH)
  {
    M5.Lcd.drawXBitmap(ICON_ETH_X, ICON_Y, ETH_icon, ICON_WIDTH, ICON_HEIGHT, ICON_ETH_COLOR, ICON_BACKCOLOR);
  }
  else
  {
    M5.Lcd.drawXBitmap(ICON_ETH_X, ICON_Y, ETH_icon, ICON_WIDTH, ICON_HEIGHT, ICON_BACKCOLOR_INACTIVE, ICON_BACKCOLOR);
  }
  if (WiFi)
  {
    M5.Lcd.drawXBitmap(ICON_WIFI_X, ICON_Y, WiFi_icon, ICON_WIDTH, ICON_HEIGHT, ICON_WIFI_COLOR, ICON_BACKCOLOR);
  }
  else
  {
    M5.Lcd.drawXBitmap(ICON_WIFI_X, ICON_Y, WiFi_icon, ICON_WIDTH, ICON_HEIGHT, ICON_BACKCOLOR_INACTIVE, ICON_BACKCOLOR);
  }
  if (AP)
  {
    M5.Lcd.drawXBitmap(ICON_AP_X, ICON_Y, AP_icon, ICON_WIDTH, ICON_HEIGHT, ICON_AP_COLOR, ICON_BACKCOLOR);
  }
  else
  {
    M5.Lcd.drawXBitmap(ICON_AP_X, ICON_Y, AP_icon, ICON_WIDTH, ICON_HEIGHT, ICON_BACKCOLOR_INACTIVE, ICON_BACKCOLOR);
  }

  // update dynamic icons every 1000 ms
  static unsigned long refreshIconsPM = 0;
  static bool TickTack = true;
  unsigned long refreshIconsCM = millis();
  if (refreshIconsCM - refreshIconsPM >= 1000)
  {
    refreshIconsPM = refreshIconsCM;
    TickTack = !TickTack;

    if (Clock)
    {
      M5.Lcd.setTextColor(DATE_TIME_COLOR, ICON_BACKCOLOR); // Bei sich ändernden Texten Hintergrund mitangeben!!!!
      M5.Lcd.setTextSize(1);
      M5.Lcd.setCursor(DATE_TIME_X, DATE_TIME_Y);
      M5.Lcd.printf("%02i:%02i:%02i", 9, 30, 05);
      M5.Lcd.setCursor(DATE_TIME_X, DATE_TIME_Y + 12);
      M5.Lcd.printf("%02i-%02i-%04i", 02, 12, 2021);
    }
  }

  if (Info && TickTack)
  {
    M5.Lcd.drawXBitmap(ICON_INFO_X, ICON_Y, Info_icon, ICON_WIDTH, ICON_HEIGHT, ICON_INFO_COLOR, ICON_BACKCOLOR);
  }
  else
  {
    M5.Lcd.drawXBitmap(ICON_INFO_X, ICON_Y, Info_icon, ICON_WIDTH, ICON_HEIGHT, ICON_BACKCOLOR_INACTIVE, ICON_BACKCOLOR);
  }
  if (Warning && TickTack)
  {
    M5.Lcd.drawXBitmap(ICON_WARNING_X, ICON_Y, Warning_icon, ICON_WIDTH, ICON_HEIGHT, ICON_WARNING_COLOR, ICON_BACKCOLOR);
  }
  else
  {
    M5.Lcd.drawXBitmap(ICON_WARNING_X, ICON_Y, Warning_icon, ICON_WIDTH, ICON_HEIGHT, ICON_BACKCOLOR_INACTIVE, ICON_BACKCOLOR);
  }
  if (Error && TickTack)
  {
    M5.Lcd.drawXBitmap(ICON_ERROR_X, ICON_Y, Error_icon, ICON_WIDTH, ICON_HEIGHT, ICON_ERROR_COLOR, ICON_BACKCOLOR);
  }
  else
  {
    M5.Lcd.drawXBitmap(ICON_ERROR_X, ICON_Y, Error_icon, ICON_WIDTH, ICON_HEIGHT, ICON_BACKCOLOR_INACTIVE, ICON_BACKCOLOR);
  }
}

void UI_progressBar(int x, int y, int w, int h, uint8_t val100pcnt, uint32_t color)
{
  M5.Lcd.fillRect(x, y, w * (((float)val100pcnt) / 100.0), h, color); // w * (((float)val100pcnt) / 100.0) = scale 100% to screenwidth px
}

void UI_deleteCanvas()
{
  M5.Lcd.fillRect(CANVAS_X, CANVAS_Y, CANVAS_WIDTH, CANVAS_HEIGHT, CANVAS_BACKGROUND); // x0,y0 are at top left
  M5.Lcd.drawRoundRect(CANVAS_X, CANVAS_Y, CANVAS_WIDTH, CANVAS_HEIGHT, 6, CANVAS_FRAME_COLOR);
}

void UI_drawHeader(const char *Title, bool WiFi, bool LAN, bool AP, bool CLOCK, bool BATTERY)
{
  M5.Lcd.fillRect(0, 0, SCREEN_WIDTH, HEADER_HEIGHT, HEADER_BACKGROUND);

  M5.Lcd.setTextColor(HEADER_TEXT_COLOR, HEADER_BACKGROUND);
  M5.Lcd.setTextSize(2);
  M5.Lcd.drawString(Title, HEADER_TEXT_X, HEADER_TEXT_Y);
}

void UI_drawFooter(const char *Btn1, const char *Btn2, const char *Btn3)
{
  M5.Lcd.setTextSize(2);
  // M5.Lcd.setFreeFont(FSS9);
  M5.Lcd.setTextColor(BUTTON_TEXT_COLOR, BUTTON_COLOR);

  // paint Button
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
    {
      M5.Lcd.fillRoundRect((i * space), HEADER_HEIGHT, space, MENU_HEIGHT + 5, 0, MENU_COLOR); // draw active item +5px at bottom
      M5.Lcd.setTextColor(MENU_TEXT_COLOR, MENU_COLOR);                                        // if text changes (eg. temperature value) explicit add canvas color as background color
    }
    else
    {
      M5.Lcd.fillRect((i * space), HEADER_HEIGHT + MENU_HEIGHT, space, 5, CANVAS_BACKGROUND);       // delete space below from menue item
      M5.Lcd.fillRoundRect((i * space), HEADER_HEIGHT, space, MENU_HEIGHT, 0, MENU_INACTIVE_COLOR); // draw inactive item
      M5.Lcd.setTextColor(MENU_TEXT_COLOR, MENU_INACTIVE_COLOR);                                    // if text changes (eg. temperature value) explicit add canvas color as background color
    }
    M5.Lcd.drawString(screenName[i], i * space + MENU_TEXT_X, HEADER_HEIGHT + 12);
  }
}

//  ------------------------------------------------------------------
//  --------------------------- SCREENS ------------------------------
//  ------------------------------------------------------------------
void UI_screenHome()
{
  M5.Lcd.setTextColor(CANVAS_TEXT_COLOR, CANVAS_BACKGROUND); // Bei sich ändernden Texten Hintergrund mitangeben!!!!
  M5.Lcd.setTextSize(2);
  M5.Lcd.drawString("Welcome to USS!", PAGE_X, PAGE_Y);
  M5.Lcd.drawString("the UniversalSensorSystem", PAGE_X, PAGE_Y + 20);
  M5.Lcd.drawString("--> " + String(millis()), PAGE_X, PAGE_Y + 40);
}

void UI_screenENV()
{
  /* //INFO                                                                          .
  If you use drawString(), drawNumber() and drawFloat() you can use setPadding()
  to automatically overwite old digits and text with background.
  See TFT_Padding_demo example.
  https: // github.com/Bodmer/TFT_eSPI/issues/350#issuecomment-488424620

  M5.Lcd.print("÷C");                      // prints °C  https://github.com/Bodmer/TFT_eSPI/issues/350#issuecomment-860571653
  M5.Lcd.drawString("`C", 50, 100, 2);     // prints °C  https://github.com/Bodmer/TFT_eSPI/issues/350#issuecomment-860571653

  */

  M5.Lcd.setCursor(PAGE_X, PAGE_Y);
  M5.Lcd.setTextColor(CANVAS_TEXT_COLOR, CANVAS_BACKGROUND); // Bei sich ändernden Texten Hintergrund mitangeben!!!!
  M5.Lcd.setTextSize(2);

  // use global json ProbeData for visualisation
  M5.Lcd.printf("T 1=%5.1f÷C  T 2=%5.1f÷C", (millis() % 128) * 0.1, (millis() % 118) * 0.1);
  M5.Lcd.setCursor(PAGE_X, PAGE_Y + 20);
  M5.Lcd.printf("T 3=%5.1f÷C  T 4=%5.1f÷C", (millis() % 228) * 0.1, (millis() % 118) * 0.1);
  M5.Lcd.setCursor(PAGE_X, PAGE_Y + 40);
  M5.Lcd.printf("T 5=%5.1f÷C  T 6=%5.1f÷C", (millis() % 28) * 0.1, (millis() % 118) * 0.1);
  M5.Lcd.setCursor(PAGE_X, PAGE_Y + 60);
  M5.Lcd.printf("T 7=%5.1f÷C  T 8=%5.1f÷C", (millis() % 58) * 0.1, (millis() % 118) * 0.1);

  // M5.Lcd.drawString("Temperature 1: " + String((millis() % 128) * 0.1) + " C  ", 10, HEADER_HEIGHT + 30 + 10);
  // M5.Lcd.drawString("Temperature 2: " + String((millis() % 118) * 0.1) + " C  ", 10, HEADER_HEIGHT + 30 + 30);
  // M5.Lcd.drawString("Temperature 3: " + String((millis() % 108) * 0.1) + " C  ", 10, HEADER_HEIGHT + 30 + 50);
  // M5.Lcd.drawString("Humidity:      " + String((millis() % 102)) + " %   ", 10, HEADER_HEIGHT + 30 + 70);
}

void UI_screenENVgraph()
{
  M5.Lcd.setCursor(PAGE_X, PAGE_Y);
  M5.Lcd.setTextColor(CANVAS_TEXT_COLOR, CANVAS_BACKGROUND); // Bei sich ändernden Texten Hintergrund mitangeben!!!!
  M5.Lcd.setTextSize(3);
  M5.Lcd.print("ENV_GRAPH ");
  M5.Lcd.println(millis());
}

void UI_screenLOG()
{
  M5.Lcd.setCursor(PAGE_X, PAGE_Y);
  M5.Lcd.setTextColor(CANVAS_TEXT_COLOR, CANVAS_BACKGROUND); // Bei sich ändernden Texten Hintergrund mitangeben!!!!
  M5.Lcd.setTextSize(3);
  M5.Lcd.print("LOG");
}

void UI_screenSysInfo()
{
  M5.Lcd.setCursor(PAGE_X, PAGE_Y);
  M5.Lcd.setTextColor(CANVAS_TEXT_COLOR, CANVAS_BACKGROUND); // Bei sich ändernden Texten Hintergrund mitangeben!!!!
  M5.Lcd.setTextSize(3);
  M5.Lcd.print("SYSINFO");
}
//  ------------------------------------------------------------------
//  ------------------------------------------------------------------
//  ------------------------------------------------------------------

void UI_showTimeoutProgressLCD(int16_t progress, int16_t max)
{
  uint8_t i;
  if ((progress == 0) || (max == 0))
  {
    i = 0; // div zero
  }
  else
  {
    i = progress * 100 / max; // scale progress to 100% as int division (result is int)
  }
  // Serial.printf("\n i=%i progress=%i calcF=%f,calcI=%i", i, progress, i * (100.0 / ((float)max)), i * (100.0 / ((float)max)));
  UI_progressBar(0, HEADER_HEIGHT - 4, SCREEN_WIDTH, 4, i, TFT_MAROON);
}

void UI_restartTimerLCD()
{
  timerLCD = 0;
  UI_drawHeader(HEADER_TITLE, 0, 0, 0, 0, 0); // repaint header, to clear embedded progress bar !!
  UI_showTimeoutProgressLCD(0, LCD_TIMEOUT);
  M5.Lcd.setBrightness(LCD_BRIGHTNESS); // set default brightness
}

void UI_timeoutLCD()
{
  // After timeout (e.g. 30 s) display will get dark
  // update screen every 'refresh' ms
  //
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

    void showWifiStrength(); // DEBUG                                                                                                   .

    // LCD brightnes fade out...
    // Brightness (0: Off - 255: Full)
    if (timerLCD == LCD_TIMEOUT / 2) // at half timeout set brightness to 50%
    {
      M5.Lcd.setBrightness(LCD_BRIGHTNESS / 2);
    }
    if (timerLCD == LCD_TIMEOUT * 80 / 100) // at 80% start to fade out
    {
      M5.Lcd.setBrightness(80);
    }
    if (timerLCD == LCD_TIMEOUT * 85 / 100)
    {
      M5.Lcd.setBrightness(70);
    }
    if (timerLCD == LCD_TIMEOUT * 90 / 100)
    {
      M5.Lcd.setBrightness(60);
    }
    if (timerLCD == LCD_TIMEOUT * 95 / 100)
    {
      M5.Lcd.setBrightness(30);
    }
    if (timerLCD == LCD_TIMEOUT * 97 / 100)
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
    UI_screenHome();
    break;
  case ENV:
    UI_screenENV();
    break;
  case ENV_GRAPH:
    UI_screenENVgraph();
    break;
  case LOG:
    UI_screenLOG();
    break;
  case SYSINFO:
    UI_screenSysInfo();
    break;
  }
}

bool e, w, a, in, wa, er, cl; // DEBUG

void UI_doHandleTFT(int16_t refresh)
{
  M5.update();
  if (M5.BtnA.wasReleased() || refresh == 0) // refresh = 0 means 1st start with default screen
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

    e = random(0, 2);  // DEBUG
    w = random(0, 2);  // DEBUG
    a = random(0, 2);  // DEBUG
    in = random(0, 2); // DEBUG
    wa = random(0, 2); // DEBUG
    er = random(0, 2); // DEBUG
    cl = random(0, 2); // DEBUG
  }

  UI_drawIcons(e, w, a, in, wa, er, cl); // DEBUG
}

void UI_setupTFT()
{
  M5.Lcd.setBrightness(LCD_BRIGHTNESS); // set default brightness
  M5.Lcd.fillScreen(SCREEN_BACKGROUND); // set default background color
  UI_doHandleTFT(0);                    // start with default screen
}