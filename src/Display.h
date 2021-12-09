#ifndef DISPLAY_h
#define DISPLAY_h

#include <Arduino.h>

#define useM5STACK // choose between M5Stack and TFT_eSPI

// generate a global variable "TFT" for M5.Lcd !!!!
#ifdef useM5STACK      //
#include <M5Stack.h>   // compiles with the M5Stack variant of TFT_eSPI () library
extern M5Display &TFT; //
#else                  //
#include <TFT_eSPI.h>  // and the original library as well
extern TFT_eSPI TFT;
#endif

#include "ytGraph.h"
#include "TFTTerminal.h"

// global definitions
#define REFRESH_RATE 3000  // screen refreshrate in ms
#define LCD_TIMEOUT 300    // 300 sec
#define LCD_BRIGHTNESS 250 // 0 .. 255  -->  default value, after timeout it is zero!

// https://www.barth-dev.de/online/rgb565-color-picker/
#define TFT_GRAY 0x8410
#define TFT_LIME 0x07FF
#define TFT_AQUA 0x04FF
#define BACKGROUND 0xC638

// screen definitions
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
#define SCREEN_BACKGROUND TFT_LIGHTGREY

// header definitions
#define HEADER_TITLE "USS V1.0"

#define HEADER_HEIGHT 25
#define HEADER_BACKGROUND 0x52AA // TFT_DARKGREY
#define HEADER_TEXT_X 3
#define HEADER_TEXT_Y 5
#define HEADER_TEXT_COLOR TFT_BLACK

// Icon definitions
#define ICON_Y 5
#define ICON_ETH_X 130
#define ICON_WIFI_X ICON_ETH_X + 20
#define ICON_AP_X ICON_WIFI_X + 20
#define ICON_INFO_X ICON_AP_X + 20
#define ICON_WARNING_X ICON_INFO_X + 20
#define ICON_ERROR_X ICON_WARNING_X + 20

#define ICON_BACKCOLOR HEADER_BACKGROUND
#define ICON_BACKCOLOR_INACTIVE BUTTON_COLOR

#define ICON_ETH_COLOR CANVAS_BACKGROUND
#define ICON_WIFI_COLOR CANVAS_BACKGROUND
#define ICON_AP_COLOR CANVAS_BACKGROUND
#define ICON_INFO_COLOR TFT_BLUE
#define ICON_WARNING_COLOR TFT_YELLOW
#define ICON_ERROR_COLOR TFT_RED

#define DATE_TIME_X ICON_WARNING_X + 50
#define DATE_TIME_Y 3
#define DATE_TIME_COLOR TFT_BLACK

// menu definitions
#define MENU_HEIGHT 25
#define MENU_TEXT_X 4
#define MENU_TEXT_Y HEADER_HEIGHT + 10
#define MENU_TEXT_COLOR TFT_BLACK
#define MENU_COLOR TFT_DARKGREY
#define MENU_INACTIVE_COLOR 0x9CF3

// footer definitions
#define FOOTER_HEIGHT 30
#define FOOTER_BACKGROUND TFT_LIGHTGREY

// button definitions
#define BUTTON_COLOR TFT_DARKGREY
#define BUTTON_WIDTH 60
#define BUTTON_HEIGHT 25
#define BUTTON_Y 212
#define BUTTON_1_X 35
#define BUTTON_2_X 150
#define BUTTON_3_X 220

#define BUTTON_TEXT_COLOR TFT_BLACK
#define BUTTON_TEXT_INACTIVE_COLOR TFT_LIGHTGREY
#define BUTTON_TEXT_Y BUTTON_Y + 6
#define BUTTON_1_TEXT_X BUTTON_1_X + 22
#define BUTTON_2_TEXT_X BUTTON_2_X + 28
#define BUTTON_3_TEXT_X BUTTON_3_X + 28

// canvas definitions
#define CANVAS_X 5
#define CANVAS_Y HEADER_HEIGHT + MENU_HEIGHT + 5
#define CANVAS_WIDTH SCREEN_WIDTH - 10
#define CANVAS_HEIGHT SCREEN_HEIGHT - HEADER_HEIGHT - FOOTER_HEIGHT - MENU_HEIGHT - 10 //

#define CANVAS_FRAME_COLOR TFT_DARKGREY
#define CANVAS_TEXT_COLOR TFT_MAROON
#define CANVAS_BACKGROUND 0xBDD7 // TFT_LIGHTGREY

// page definition
#define PAGE_X CANVAS_X + 4
#define PAGE_Y CANVAS_Y + 6

// global Variables
extern int8_t showScreen; // active shown screen - default display mode 0=HOME 1=GRAPH 2=ENV_GRAPH

const int noOfScreens = 5; // max 6-7 because of menue text length ...
const char screenName[noOfScreens][15] = {
    {"  Home   "}, // max 9 chars if 5 menu items are used
    {"Env. data"},
    {"Env Graph"},
    {" Messages"},
    {" Sysinfo "}};

enum Screens
{
    HOME,
    ENV,
    ENV_GRAPH,
    LOG,
    SYSINFO
};

void UI_setupTFT();                   // 1st setup
void UI_doHandleTFT(int16_t refresh); // must be in loop()!

void UI_TerminalPrint(MsgType mt, const char *msg);
void UI_GraphPrint(GraphValues_t &GV);
void UI_EnvPrint(GraphValues_t &GV);

#endif // DISPLAY_h
