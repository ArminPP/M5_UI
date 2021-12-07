#include <Arduino.h>
#include <M5Stack.h>

#include "Display.h"
#include "icons.h"
// NO FREE FONTS! Flicker when updating :-(
// #include "Free_Fonts.h" // Include the header file attached to this sketch

// Pointer for TFT for using both libs with the same code
#ifdef useM5STACK
M5Display &TFT = M5.Lcd;
#else
TFT_eSPI TFT = TFT_eSPI();
#endif

TFT_eSprite Graph = TFT_eSprite(&TFT); // canvas of graph, static & dynamic (grid and lines)
TFT_eSprite xAxis = TFT_eSprite(&TFT); // canvas of scrolling x axis

TFT_eSprite Terminal = TFT_eSprite(&TFT); // scrolling Terminal

// global Variables
int8_t showScreen = 0; // Define the variable: default display mode 0=HOME 1=ENV 2=ENV_GRAPH

// internal Variables
int16_t timerLCD = 0;

static int16_t oy1 = 0; // static, local or global var to store the last y position in graph
static int16_t oy2 = 0; // is need after each drawing of the historical buffer!
static int16_t oy3 = 0;
static int16_t oy4 = 0;
static int16_t oy5 = 0;
static int16_t ox1 = 0; // for each new drwaing of historical data, x axis starts at zero
static int16_t ox2 = 0;
static int16_t ox3 = 0;
static int16_t ox4 = 0;
static int16_t ox5 = 0;
static int16_t LastXGridLinePos = 0; // for dynamic scrolling - to add new gridline at proper place

// TODO TEST                                                                                .
// TODO TEST                                                                                .
char *getAllFreeHeap()
{
  static char freeH[80]{}; // returns the formated free heap space
  sprintf(freeH, "Size:%.2fkB Free:%.2fkB Min:%.2fkB Max:%.2fkB",
          ESP.getHeapSize() / 1024.0,
          ESP.getFreeHeap() / 1024.0,
          ESP.getMinFreeHeap() / 1024.0,
          ESP.getMaxAllocHeap() / 1024.0);
  return freeH;
}

// TODO TEST                                                                                .
// TODO TEST                                                                                .
void printFreeHeap() // for debugging issues
{
  Serial.println(getAllFreeHeap());
}

void UI_SetupTerminal()
{
  Terminal.setColorDepth(4);                              // max 16 graph lines with different colors
  Terminal.createSprite(TERMINAL_WIDTH, TERMINAL_HEIGTH); // height = width at M5Stack (landscape mode!)

  Terminal.fillSprite(TERMINAL_BGRND_COLOR);                                           // Note: Sprite is filled with black when created
  Terminal.setScrollRect(0, 0, TERMINAL_WIDTH, TERMINAL_HEIGTH, TERMINAL_BGRND_COLOR); // INFO: to choose a different sroll color than black/white it is mandatory to set a scrollRect !!!!
}

void UI_DrawTerminal()
{
  TerminalPrint(Terminal, WARNING, "SuperWarnung");

  TerminalPrint(Terminal, ERROR, ".      SuperError");

  TerminalPrint(Terminal, INFO, "__________________SuperINFO");

  TerminalPrint(Terminal, NONE, "_____________________SuperNONE");

  TerminalPrint(Terminal, NONE, "_VERRY LONG MESSAGE 34959573195719458"); // max length of a single line
}

// #################################################################################################################
// #################################################################################################################
// #################################################################################################################
// #################################################################################################################
// #################################################################################################################

void printLegend()
{
  TFT.setTextSize(1);
  TFT.setTextColor(TFT_BLACK);
  TFT.fillRect(10, 60, 8, 8, LEGEND_1_COLOR);
  TFT.setCursor(20, 60);
  TFT.print(LEGEND_1);
  TFT.fillRect(75, 60, 8, 8, LEGEND_2_COLOR);
  TFT.setCursor(85, 60);
  TFT.print(LEGEND_2);
  TFT.fillRect(140, 60, 8, 8, LEGEND_3_COLOR);
  TFT.setCursor(150, 60);
  TFT.print(LEGEND_3);
  TFT.fillRect(200, 60, 8, 8, LEGEND_4_COLOR);
  TFT.setCursor(210, 60);
  TFT.print(LEGEND_4);
  TFT.fillRect(260, 60, 8, 8, LEGEND_5_COLOR);
  TFT.setCursor(270, 60);
  TFT.print(LEGEND_5);
}

void UI_SetupGraph()
{
  Graph.setColorDepth(4);                          // max 16 graph lines with different colors
  Graph.createSprite(SPRITE_WIDTH, SPRITE_HEIGTH); // height = width at M5Stack (landscape mode!)

  Graph.fillSprite(GRAPH_BGRND_COLOR);                                       // Note: Sprite is filled with black when created
  Graph.setScrollRect(0, 0, SPRITE_WIDTH, SPRITE_HEIGTH, GRAPH_BGRND_COLOR); // INFO: to choose a different sroll color than black/white it is mandatory to set a scrollRect !!!!

  xAxis.setColorDepth(4); // could not save some kBytes, with depth(1) only black&white colors are available ?!
  xAxis.createSprite(X_AXIS_WIDTH, X_AXIS_HEIGTH);

  xAxis.fillSprite(CANVAS_BACKGROUND);                                       // Note: Sprite is filled with black when created
  xAxis.setScrollRect(0, 0, X_AXIS_WIDTH, X_AXIS_HEIGTH, CANVAS_BACKGROUND); // INFO: to choose a different sroll color than black/white it is mandatory to set a scrollRect !!!!

  ytGraphDrawGridXaxis(Graph, xAxis, LastXGridLinePos); // draw the grid

  // move starting point to the right side of the grap canvas
  // so the diagonal lines from the left side are now gon,
  // when no historicalvalues are drawed!
  ox1 = SPRITE_WIDTH; // correction of the last point in graph after scrolling to the left
  ox2 = SPRITE_WIDTH;
  ox3 = SPRITE_WIDTH;
  ox4 = SPRITE_WIDTH;
  ox5 = SPRITE_WIDTH;
}

void UI_DrawGraph()
{
  ytGraphDrawYaxisFrame(TFT); // draw the y axis and the frame

  printFreeHeap(); // DEBUG
  printLegend();

  int16_t scrollX = round(GRAPH_WIDTH / SAMPLE_COUNT); // scroll one sample to the left

  ox1 -= scrollX; // correction of the last point in graph after scrolling to the left
  ox2 -= scrollX;
  ox3 -= scrollX;
  ox4 -= scrollX;
  ox5 -= scrollX;

  Graph.scroll(-scrollX);
  xAxis.scroll(-scrollX);

  ytGraphDrawDynamicGrid(Graph, xAxis, ox4, LastXGridLinePos);

  // add to the the end of the buffer the latest sensor reading
  int16_t temp1 = lround(random(2, 5));
  int16_t temp2 = lround(random(8, 12));
  int16_t Co = lround(random(15, 20));
  int16_t humidity = random(30, 35);
  int16_t tvoc = random(40, 75);

  ytGraph(Graph, SAMPLE_COUNT, temp1, LEGEND_1_COLOR, ox1, oy1);
  ytGraph(Graph, SAMPLE_COUNT, temp2, LEGEND_2_COLOR, ox2, oy2);
  ytGraph(Graph, SAMPLE_COUNT, Co, LEGEND_3_COLOR, ox3, oy3);
  ytGraph(Graph, SAMPLE_COUNT, humidity, LEGEND_4_COLOR, ox4, oy4);
  ytGraph(Graph, SAMPLE_COUNT, tvoc, LEGEND_5_COLOR, ox5, oy5);

  xAxis.pushSprite(X_AXIS_LEFT_X, X_AXIS_UPPER_Y); // no Background color
  Graph.pushSprite(SPRITE_LEFT_X, SPRITE_UPPER_Y); // left upper position
}

void UI_drawIcons(bool ETH, bool WiFi, bool AP, bool Info, bool Warning, bool Error, bool Clock)
// ETH, WiFi, AP are static icons, the change only if the connection changes
// 'Info, Warning and Error are dynamic icons, if they are triggered they will blink every second
// Clock updates every second... (maybe this ist to fast?)
//
{
  if (ETH)
  {
    TFT.drawXBitmap(ICON_ETH_X, ICON_Y, ETH_icon, ICON_WIDTH, ICON_HEIGHT, ICON_ETH_COLOR, ICON_BACKCOLOR);
  }
  else
  {
    TFT.drawXBitmap(ICON_ETH_X, ICON_Y, ETH_icon, ICON_WIDTH, ICON_HEIGHT, ICON_BACKCOLOR_INACTIVE, ICON_BACKCOLOR);
  }
  if (WiFi)
  {
    TFT.drawXBitmap(ICON_WIFI_X, ICON_Y, WiFi_icon, ICON_WIDTH, ICON_HEIGHT, ICON_WIFI_COLOR, ICON_BACKCOLOR);
  }
  else
  {
    TFT.drawXBitmap(ICON_WIFI_X, ICON_Y, WiFi_icon, ICON_WIDTH, ICON_HEIGHT, ICON_BACKCOLOR_INACTIVE, ICON_BACKCOLOR);
  }
  if (AP)
  {
    TFT.drawXBitmap(ICON_AP_X, ICON_Y, AP_icon, ICON_WIDTH, ICON_HEIGHT, ICON_AP_COLOR, ICON_BACKCOLOR);
  }
  else
  {
    TFT.drawXBitmap(ICON_AP_X, ICON_Y, AP_icon, ICON_WIDTH, ICON_HEIGHT, ICON_BACKCOLOR_INACTIVE, ICON_BACKCOLOR);
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
      TFT.setTextColor(DATE_TIME_COLOR, ICON_BACKCOLOR); // Bei sich ändernden Texten Hintergrund mitangeben!!!!
      TFT.setTextSize(1);
      TFT.setCursor(DATE_TIME_X, DATE_TIME_Y);
      TFT.printf("%02i:%02i:%02i", 9, 30, 05);
      TFT.setCursor(DATE_TIME_X, DATE_TIME_Y + 12);
      TFT.printf("%02i-%02i-%04i", 02, 12, 2021);
    }
  }

  if (Info && TickTack)
  {
    TFT.drawXBitmap(ICON_INFO_X, ICON_Y, Info_icon, ICON_WIDTH, ICON_HEIGHT, ICON_INFO_COLOR, ICON_BACKCOLOR);
  }
  else
  {
    TFT.drawXBitmap(ICON_INFO_X, ICON_Y, Info_icon, ICON_WIDTH, ICON_HEIGHT, ICON_BACKCOLOR_INACTIVE, ICON_BACKCOLOR);
  }
  if (Warning && TickTack)
  {
    TFT.drawXBitmap(ICON_WARNING_X, ICON_Y, Warning_icon, ICON_WIDTH, ICON_HEIGHT, ICON_WARNING_COLOR, ICON_BACKCOLOR);
  }
  else
  {
    TFT.drawXBitmap(ICON_WARNING_X, ICON_Y, Warning_icon, ICON_WIDTH, ICON_HEIGHT, ICON_BACKCOLOR_INACTIVE, ICON_BACKCOLOR);
  }
  if (Error && TickTack)
  {
    TFT.drawXBitmap(ICON_ERROR_X, ICON_Y, Error_icon, ICON_WIDTH, ICON_HEIGHT, ICON_ERROR_COLOR, ICON_BACKCOLOR);
  }
  else
  {
    TFT.drawXBitmap(ICON_ERROR_X, ICON_Y, Error_icon, ICON_WIDTH, ICON_HEIGHT, ICON_BACKCOLOR_INACTIVE, ICON_BACKCOLOR);
  }
}

void UI_progressBar(int x, int y, int w, int h, uint8_t val100pcnt, uint32_t color)
{
  TFT.fillRect(x, y, w * (((float)val100pcnt) / 100.0), h, color);
}

void UI_deleteCanvas()
{
  TFT.fillRect(CANVAS_X, CANVAS_Y, CANVAS_WIDTH, CANVAS_HEIGHT, CANVAS_BACKGROUND); // x0,y0 are at top left
  TFT.drawRoundRect(CANVAS_X, CANVAS_Y, CANVAS_WIDTH, CANVAS_HEIGHT, 6, CANVAS_FRAME_COLOR);
}

void UI_drawHeader(const char *Title, bool WiFi, bool LAN, bool AP, bool CLOCK, bool BATTERY)
{
  TFT.fillRect(0, 0, SCREEN_WIDTH, HEADER_HEIGHT, HEADER_BACKGROUND);

  TFT.setTextColor(HEADER_TEXT_COLOR, HEADER_BACKGROUND);
  TFT.setTextSize(2);
  TFT.drawString(Title, HEADER_TEXT_X, HEADER_TEXT_Y);
}

void UI_drawFooter(const char *Btn1, const char *Btn2, const char *Btn3)
{
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
  TFT.setTextSize(2);
  TFT.drawString("Welcome to USS!", PAGE_X, PAGE_Y);
  TFT.drawString("the UniversalSensorSystem", PAGE_X, PAGE_Y + 20);
  TFT.drawString("--> " + String(millis()), PAGE_X, PAGE_Y + 40);
}

void UI_screenENV()
{
  /* //INFO                                                                          .
  If you use drawString(), drawNumber() and drawFloat() you can use setPadding()
  to automatically overwite old digits and text with background.
  See TFT_Padding_demo example.
  https: // github.com/Bodmer/TFT_eSPI/issues/350#issuecomment-488424620

  TFT.print("÷C");                      // prints °C  https://github.com/Bodmer/TFT_eSPI/issues/350#issuecomment-860571653
  TFT.drawString("`C", 50, 100, 2);     // prints °C  https://github.com/Bodmer/TFT_eSPI/issues/350#issuecomment-860571653

  */

  TFT.setCursor(PAGE_X, PAGE_Y);
  TFT.setTextColor(CANVAS_TEXT_COLOR, CANVAS_BACKGROUND); // Bei sich ändernden Texten Hintergrund mitangeben!!!!
  TFT.setTextSize(2);

  // use global json ProbeData for visualisation
  TFT.printf("T 1=%5.1f÷C  T 2=%5.1f÷C", (millis() % 128) * 0.1, (millis() % 118) * 0.1);
  TFT.setCursor(PAGE_X, PAGE_Y + 20);
  TFT.printf("T 3=%5.1f÷C  T 4=%5.1f÷C", (millis() % 228) * 0.1, (millis() % 118) * 0.1);
  TFT.setCursor(PAGE_X, PAGE_Y + 40);
  TFT.printf("T 5=%5.1f÷C  T 6=%5.1f÷C", (millis() % 28) * 0.1, (millis() % 118) * 0.1);
  TFT.setCursor(PAGE_X, PAGE_Y + 60);
  TFT.printf("T 7=%5.1f÷C  T 8=%5.1f÷C", (millis() % 58) * 0.1, (millis() % 118) * 0.1);
}

// void UI_screenENVgraph()
// {
//   TFT.setCursor(PAGE_X, PAGE_Y);
//   TFT.setTextColor(CANVAS_TEXT_COLOR, CANVAS_BACKGROUND); // Bei sich ändernden Texten Hintergrund mitangeben!!!!
//   TFT.setTextSize(3);
//   TFT.print("ENV_GRAPH ");
//   TFT.println(millis());
// }

void UI_screenLOG()
{
  TFT.setCursor(PAGE_X, PAGE_Y);
  TFT.setTextColor(CANVAS_TEXT_COLOR, CANVAS_BACKGROUND); // Bei sich ändernden Texten Hintergrund mitangeben!!!!
  TFT.setTextSize(3);
  TFT.print("LOG");
}

void UI_screenSysInfo()
{
  TFT.setCursor(PAGE_X, PAGE_Y);
  TFT.setTextColor(CANVAS_TEXT_COLOR, CANVAS_BACKGROUND); // Bei sich ändernden Texten Hintergrund mitangeben!!!!
  TFT.setTextSize(3);
  TFT.print("SYSINFO");
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
  TFT.setBrightness(LCD_BRIGHTNESS); // set default brightness
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
      TFT.setBrightness(LCD_BRIGHTNESS / 2);
    }
    if (timerLCD == LCD_TIMEOUT * 80 / 100) // at 80% start to fade out
    {
      TFT.setBrightness(80);
    }
    if (timerLCD == LCD_TIMEOUT * 85 / 100)
    {
      TFT.setBrightness(70);
    }
    if (timerLCD == LCD_TIMEOUT * 90 / 100)
    {
      TFT.setBrightness(60);
    }
    if (timerLCD == LCD_TIMEOUT * 95 / 100)
    {
      TFT.setBrightness(30);
    }
    if (timerLCD == LCD_TIMEOUT * 97 / 100)
    {
      TFT.setBrightness(20);
    }
    if (timerLCD == LCD_TIMEOUT)
    {
      TFT.setBrightness(0);
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
  case HOME:
    UI_screenHome();
    break;
  case ENV:
    UI_screenENV();
    break;
  case ENV_GRAPH:
  {
    // ytGraphDrawYaxisFrame(TFT); // draw the y axis and the frame once
    UI_DrawGraph();
    break;
  }
  case LOG:
    // UI_screenLOG();
    UI_DrawTerminal();
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
    cl = true;         // DEBUG
  }

  UI_drawIcons(e, w, a, in, wa, er, cl); // DEBUG
}

void UI_setupTFT()
{
  TFT.setBrightness(LCD_BRIGHTNESS); // set default brightness
  TFT.fillScreen(SCREEN_BACKGROUND); // set default background color

  UI_SetupGraph();
  UI_DrawGraph();

  UI_SetupTerminal();

  UI_doHandleTFT(0); // start with default screen
}