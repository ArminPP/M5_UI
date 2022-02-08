#include <Arduino.h>
#include <M5Stack.h>

#include "Display.h"
#include "icons.h"
#include "SignalProcessing.h"

// NO FREE FONTS! Flickering when updating :-(
// #include "Free_Fonts.h" // Include the header file attached to this sketch

// Pointer for TFT for using both libs with the same code
#ifdef useM5STACK
M5Display &TFT = M5.Lcd;
#else
TFT_eSPI TFT = TFT_eSPI();
#endif

#define msgTimer 3 // dynamic messages (info, warning and error are only shown for 3 s in the status bar)

TFT_eSprite Graph = TFT_eSprite(&TFT);    // canvas of graph, static & dynamic (grid and lines)
TFT_eSprite xAxis = TFT_eSprite(&TFT);    // canvas of scrolling x axis
TFT_eSprite Terminal = TFT_eSprite(&TFT); // scrolling Terminal

// global Variables
int8_t showScreen = 0; // Define the variable: default display mode 0=SCR_HOME 1=SCR_ENV 2=SCR_ENV_GRAPH

bool ico_ETH = false;
bool ico_WIFI = false;
bool ico_AP = false;
bool ico_INFO = false;
bool ico_WARN = false;
bool ico_ERR = false;
bool ico_CLK = false;

// internal Variables
int16_t timerLCD = 0;

void UI_TerminalPrint(const char *dt, Credentials::LogMsgType mt, const char *msg)
// Wrapper for Terminalprint
{
  printTerminal(Terminal, dt, mt, msg);
}

void UI_GraphPrint(GraphValues_t &GV)
// Wrapper for printGraph
{
  printGraph(Graph, xAxis, GV);
}

void UI_EnvPrint(GraphValues_t &GV)
// this screen is not integrated in the TFT cycle,
// the update rate is handled outside of this function!
//
{
  if (showScreen == SCR_ENV) // only show values if screen is active!
  {
    // INFO this doesn't work, there are some glitches if screens are swaped via btnA/C pressing...
    // TFT.setFreeFont(FF33);                                  // Select Free Serif 12 point font
    // TFT.setTextPadding(50);
    // TFT.drawFloat(GV.BMP280_Temperature, 2, PAGE_X, PAGE_Y, GFXFF);
    // (..)
    // TFT.setTextFont(1); // return to standard font

    TFT.setTextColor(CANVAS_TEXT_COLOR, CANVAS_BACKGROUND);
    TFT.setCursor(PAGE_X, PAGE_Y);
    TFT.setTextFont(1); // return to standard font
    TFT.setTextSize(2);

    // use global json ProbeData for visualisation
    TFT.printf("T 1=%5.1f÷C  T 2=%5.1f÷C", GV.BMP280_Temperature, GV.BMP280_Temperature * 0.931);
    TFT.setCursor(PAGE_X, PAGE_Y + 20);
    TFT.printf("T 3=%5.1f÷C  T 4=%5.1f÷C", GV.SHT30_Temperature, GV.SHT30_Temperature * 0.231);
    TFT.setCursor(PAGE_X, PAGE_Y + 40);
    TFT.printf("CO =%5.1f÷C  Hum=%5.1f÷C", GV.eCO2, GV.Humidity);
    TFT.setCursor(PAGE_X, PAGE_Y + 60);
    TFT.printf("Hum=%5.1f÷C  VOC=%5.1f÷C", GV.Humidity, GV.TVOC);
  }
}

// #################################################################################################################
// #################################################################################################################
// #################################################################################################################

void UI_drawIcons()
// ETH, WiFi, AP are static icons, the change only if the connection changes
// 'Info, Warning and Error are dynamic icons, if they are triggered they will be shown
// for 3 seconds, then they will be inactive due to the next event.
//
{
  // static icons
  if (ico_ETH)
  {
    TFT.drawXBitmap(ICON_ETH_X, ICON_Y, ETH_icon, ICON_WIDTH, ICON_HEIGHT, ICON_ETH_COLOR, ICON_BACKCOLOR);
  }
  else
  {
    TFT.drawXBitmap(ICON_ETH_X, ICON_Y, ETH_icon, ICON_WIDTH, ICON_HEIGHT, ICON_BACKCOLOR_INACTIVE, ICON_BACKCOLOR);
  }
  if (ico_WIFI)
  {
    TFT.drawXBitmap(ICON_WIFI_X, ICON_Y, WiFi_icon, ICON_WIDTH, ICON_HEIGHT, ICON_WIFI_COLOR, ICON_BACKCOLOR);
  }
  else
  {
    TFT.drawXBitmap(ICON_WIFI_X, ICON_Y, WiFi_icon, ICON_WIDTH, ICON_HEIGHT, ICON_BACKCOLOR_INACTIVE, ICON_BACKCOLOR);
  }
  if (ico_AP)
  {
    TFT.drawXBitmap(ICON_AP_X, ICON_Y, AP_icon, ICON_WIDTH, ICON_HEIGHT, ICON_AP_COLOR, ICON_BACKCOLOR);
  }
  else
  {
    TFT.drawXBitmap(ICON_AP_X, ICON_Y, AP_icon, ICON_WIDTH, ICON_HEIGHT, ICON_BACKCOLOR_INACTIVE, ICON_BACKCOLOR);
  }

  // dynamic icons
  static uint8_t timer_err = 0; // helper for the oneshot timer
  static uint8_t timer_info = 0;
  static uint8_t timer_warn = 0;
  if (ico_INFO)
  {
    TFT.drawXBitmap(ICON_INFO_X, ICON_Y, Info_icon, ICON_WIDTH, ICON_HEIGHT, ICON_INFO_COLOR, ICON_BACKCOLOR);
    if (timer_info == 0) // only if oneshot timer is inactive and event active...
    {                    //
      timer_info = 1;    // ...start the oneshot timer!
    }
  }
  else
  {
    TFT.drawXBitmap(ICON_INFO_X, ICON_Y, Info_icon, ICON_WIDTH, ICON_HEIGHT, ICON_BACKCOLOR_INACTIVE, ICON_BACKCOLOR);
  }
  if (ico_WARN)
  {
    TFT.drawXBitmap(ICON_WARNING_X, ICON_Y, Warning_icon, ICON_WIDTH, ICON_HEIGHT, ICON_WARNING_COLOR, ICON_BACKCOLOR);
    if (timer_warn == 0)
    {
      timer_warn = 1;
    }
  }
  else
  {
    TFT.drawXBitmap(ICON_WARNING_X, ICON_Y, Warning_icon, ICON_WIDTH, ICON_HEIGHT, ICON_BACKCOLOR_INACTIVE, ICON_BACKCOLOR);
  }
  if (ico_ERR)
  {
    TFT.drawXBitmap(ICON_ERROR_X, ICON_Y, Error_icon, ICON_WIDTH, ICON_HEIGHT, ICON_ERROR_COLOR, ICON_BACKCOLOR);
    if (timer_err == 0)
    {
      timer_err = 1;
    }
  }
  else
  {
    TFT.drawXBitmap(ICON_ERROR_X, ICON_Y, Error_icon, ICON_WIDTH, ICON_HEIGHT, ICON_BACKCOLOR_INACTIVE, ICON_BACKCOLOR);
  }

  // oneshot timer
  // on error event show the icon only for 3 seconds (msgTimer)
  static unsigned long BlinkErrPM = 0;
  unsigned long BlinkErrCM = millis();
  if (timer_err > 0)                     // if oneshot timer is started...
    if (BlinkErrCM - BlinkErrPM >= 1000) // ...every second
    {                                    //
      if (timer_err > msgTimer)          // ...check if time is over
      {                                  //
        timer_err = 0;                   // if so - stop timer...
        ico_ERR = false;                 // ...make icon inactive
      }                                  //
      else                               // otherwise...
      {                                  //
        timer_err++;                     // ...start next round of timer
      }                                  //
      BlinkErrPM = BlinkErrCM;           //
    }
  // oneshot timer
  // on warn event show the icon only for 3 seconds (msgTimer)
  static unsigned long BlinkWarnPM = 0;
  unsigned long BlinkWarnCM = millis();
  if (timer_warn > 0)
    if (BlinkWarnCM - BlinkWarnPM >= 1000)
    {
      if (timer_warn > msgTimer)
      {
        timer_warn = 0;
        ico_WARN = false;
      }
      else
      {
        timer_warn++;
      }
      BlinkWarnPM = BlinkWarnCM;
    }
  // oneshot timer
  // on info event show the icon only for 3 seconds (msgTimer)
  static unsigned long BlinkInfoPM = 0;
  unsigned long BlinkInfoCM = millis();
  if (timer_info > 0)
    if (BlinkInfoCM - BlinkInfoPM >= 1000)
    {
      if (timer_info > msgTimer)
      {
        ico_INFO = false;
        timer_info = 0;
      }
      else
      {
        timer_info++;
      }
      BlinkInfoPM = BlinkInfoCM;
    }

  // update the clock every 1000 ms
  static unsigned long refreshIconsPM = 0;
  unsigned long refreshIconsCM = millis();
  if (refreshIconsCM - refreshIconsPM >= 1000)
  {
    if (ico_CLK)
    {
      TFT.setTextColor(DATE_TIME_COLOR, ICON_BACKCOLOR);
      TFT.setTextFont(1); // return to standard font
      TFT.setTextSize(1);
      TFT.setCursor(DATE_TIME_X, DATE_TIME_Y);
      TFT.printf("%02i:%02i:%02i", 9, 30, 05);
      TFT.setCursor(DATE_TIME_X, DATE_TIME_Y + 12);
      TFT.printf("%02i-%02i-%04i", 02, 12, 2021);
    }
    refreshIconsPM = refreshIconsCM;
  }
}

void UI_progressBar(int x, int y, int w, int h, uint8_t val100pcnt, uint32_t color)
// progressbar for showing time for timeout of backlit display
{
  TFT.fillRect(x, y, w * (((float)val100pcnt) / 100.0), h, color);
}

void UI_deleteCanvas()
{
  TFT.fillRect(CANVAS_X, CANVAS_Y, CANVAS_WIDTH, CANVAS_HEIGHT, CANVAS_BACKGROUND); // x0,y0 are at top left
  TFT.drawRoundRect(CANVAS_X, CANVAS_Y, CANVAS_WIDTH, CANVAS_HEIGHT, 6, CANVAS_FRAME_COLOR);
}

void UI_drawHeader()
{
  TFT.setTextFont(1); // return to standard font
  TFT.fillRect(0, 0, SCREEN_WIDTH, HEADER_HEIGHT, HEADER_BACKGROUND);

  // TFT.setTextColor(HEADER_TEXT_COLOR, HEADER_BACKGROUND);
  // TFT.setTextSize(2);
  // TFT.drawString(HEADER_TITLE, HEADER_TEXT_X, HEADER_TEXT_Y);

 TFT.drawXBitmap(0, 0, Logo_USS, LOGO_WIDTH, LOGO_HEIGHT, LOGO_COLOR, HEADER_BACKGROUND);
}

void UI_drawFooter(const char *Btn1, const char *Btn2, const char *Btn3)
{
  TFT.setTextFont(1); // return to standard font
  TFT.setTextSize(2);
  TFT.setTextColor(BUTTON_TEXT_COLOR, BUTTON_COLOR);
  // paint Button
  if (strlen(Btn1) > 0)
  {
    TFT.fillRoundRect(BUTTON_1_X, BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT, 6, BUTTON_COLOR);  
    TFT.drawString(Btn1, BUTTON_1_TEXT_X, BUTTON_TEXT_Y);                               
  }
  else
  {
    // delete Button
    TFT.fillRoundRect(BUTTON_1_X, BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT, 6, FOOTER_BACKGROUND);
  }
  if (strlen(Btn2) > 0)
  {
    TFT.fillRoundRect(BUTTON_2_X, BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT, 6, BUTTON_COLOR);
    TFT.drawString(Btn2, BUTTON_2_TEXT_X, BUTTON_TEXT_Y);
  }
  else
  {
    // delete Button
    TFT.fillRoundRect(BUTTON_2_X, BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT, 6, FOOTER_BACKGROUND);
  }
  if (strlen(Btn3) > 0)
  {
    TFT.fillRoundRect(BUTTON_3_X, BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT, 6, BUTTON_COLOR);
    TFT.drawString(Btn3, BUTTON_3_TEXT_X, BUTTON_TEXT_Y);
  }
  else
  {
    // delete Button
    TFT.fillRoundRect(BUTTON_3_X, BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT, 6, FOOTER_BACKGROUND);
  }
}

void UI_drawMenue(int8_t activeItem)
{
  TFT.setTextFont(1); // return to standard font
  TFT.setTextSize(1);
  int16_t space = (TFT_HEIGHT / noOfScreens); //- 5
  for (int i = 0; i < noOfScreens; ++i)
  {
    if (i == activeItem)
    {
      TFT.fillRoundRect((i * space), HEADER_HEIGHT, space, MENU_HEIGHT, 5, MENU_COLOR); // draw active item +5px at bottom
      TFT.setTextColor(MENU_TEXT_COLOR, MENU_COLOR);                                    // if text changes (eg. temperature value) explicit add canvas color as background color
    }
    else
    {
      // TFT.fillRect((i * space), HEADER_HEIGHT + MENU_HEIGHT, space, 5, CANVAS_BACKGROUND);       // delete space below from menue item
      TFT.fillRoundRect((i * space), HEADER_HEIGHT, space, MENU_HEIGHT, 5, MENU_INACTIVE_COLOR); // draw inactive item
      TFT.setTextColor(MENU_TEXT_COLOR, MENU_INACTIVE_COLOR);                                    // if text changes (eg. temperature value) explicit add canvas color as background color
      TFT.drawRoundRect((i * space), HEADER_HEIGHT, space, MENU_HEIGHT, 5, CANVAS_FRAME_COLOR);  // paint a light frame on inactive menu items
    }
    TFT.drawString(screenName[i], i * space + MENU_TEXT_X, MENU_TEXT_Y);
  }
}

//  ------------------------------------------------------------------
//  --------------------------- SCREENS ------------------------------
//  ------------------------------------------------------------------

void UI_screenHome()
{
  TFT.setTextColor(CANVAS_TEXT_COLOR, CANVAS_BACKGROUND); // Bei sich ändernden Texten Hintergrund mitangeben!!!!
  TFT.setTextFont(1);                                     // return to standard font
  TFT.setTextSize(2);
  TFT.drawString("Welcome to USS!", PAGE_X, PAGE_Y + 20);
  TFT.drawString("the", PAGE_X, PAGE_Y + 40);
  TFT.drawString("Universal Sensor System", PAGE_X, PAGE_Y + 60);

  // // INFO this doesn't work, there are some glitches if screens are swaped via btnA/C pressing...
  // TFT.setFreeFont(FF33); // Select Free Serif 12 point font
  // TFT.setTextPadding(50);
  // TFT.drawString("Welcome to USS!", PAGE_X, PAGE_Y, GFXFF);
  // TFT.drawString("the UniversalSensorSystem", PAGE_X, PAGE_Y + 40, GFXFF);
  // (..)
}

void UI_screenSysInfo()
{
  TFT.setCursor(PAGE_X, PAGE_Y);
  TFT.setTextColor(CANVAS_TEXT_COLOR, CANVAS_BACKGROUND);
  TFT.setTextFont(1); // return to standard font
  TFT.setTextSize(3);
  TFT.print("SCR_SYSINFO");
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

void UI_deleteTimeoutProgressLCD()
// after restart of timeout, remove progress bar
{
  TFT.fillRect(0, HEADER_HEIGHT - 4, SCREEN_WIDTH, 4, HEADER_BACKGROUND);
}

void UI_restartTimerLCD()
{
  timerLCD = 0;
  UI_deleteTimeoutProgressLCD();
  UI_showTimeoutProgressLCD(0, Credentials::TFT_TIMEOUT);
  TFT.writecommand(ILI9341_DISPON);               // display on, saves power
  delay(200);                                     // NEW NEEDED ??????
  TFT.setBrightness(Credentials::TFT_BRIGHTNESS); // set default brightness
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

    if (timerLCD <= Credentials::TFT_TIMEOUT) // because of overflow of int_8
    {
      timerLCD += 1;
      UI_showTimeoutProgressLCD(timerLCD, Credentials::TFT_TIMEOUT);
    }

    // void showWifiStrength(); // DEBUG                                                                                                   .

    // LCD brightnes fade out...
    // Brightness (0: Off - 255: Full)
    if (timerLCD == Credentials::TFT_TIMEOUT / 2) // at half timeout set brightness to 50%
    {
      TFT.setBrightness(Credentials::TFT_BRIGHTNESS / 2);
    }
    if (timerLCD == Credentials::TFT_TIMEOUT * 80 / 100) // at 80% start to fade out
    {
      TFT.setBrightness(80);
    }
    if (timerLCD == Credentials::TFT_TIMEOUT * 85 / 100)
    {
      TFT.setBrightness(70);
    }
    if (timerLCD == Credentials::TFT_TIMEOUT * 90 / 100)
    {
      TFT.setBrightness(60);
    }
    if (timerLCD == Credentials::TFT_TIMEOUT * 95 / 100)
    {
      TFT.setBrightness(30);
    }
    if (timerLCD == Credentials::TFT_TIMEOUT * 97 / 100)
    {
      TFT.setBrightness(20);
    }
    if (timerLCD == Credentials::TFT_TIMEOUT)
    {
      TFT.setBrightness(0);              // TFT is 'off' backlight is off
      TFT.writecommand(ILI9341_DISPOFF); // should save power
    }
  }
}
void UI_showActiveScreen(uint8_t screen)
{
  // INFO this function is called every xx seconds in an interval
  // DON'T delete canvas in here
  // because in interval refreshing mode
  // there will be flickering!
  //
  switch (screen)
  {
  case SCR_HOME:
    UI_screenHome();
    break;
  case SCR_ENV:
    // is in different interval !
    break;
  case SCR_ENV_GRAPH:
  {
    showGraph(Graph, xAxis, TFT);
    break;
  }
  case SCR_MESSAGES:
    showTerminal(Terminal);
    break;
  case SCR_SYSINFO:
    UI_screenSysInfo();
    break;
  }
}

// #################################################################################################################
// #################################################################################################################
// #################################################################################################################

void UI_doHandleTFT(int16_t refresh)
{
  M5.update();
  TFT.setTextFont(1);                        // return to standard font
  TFT.setTextSize(2);                        // NEW
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

  UI_drawIcons();
}

void UI_setupTFT()
{
  TFT.setBrightness(Credentials::TFT_BRIGHTNESS); // set default brightness
  TFT.fillScreen(SCREEN_BACKGROUND);              // set default background color

  setupGraph(Graph, xAxis);
  showGraph(Graph, xAxis, TFT);

  setupTerminal(Terminal);
  showTerminal(Terminal);
  UI_drawHeader();   // paint header once
  UI_doHandleTFT(0); // start with default screen
}