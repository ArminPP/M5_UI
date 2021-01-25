#ifndef UI_h
#define UI_h

#include <M5Stack.h>

// pragma once prevents the file to be included twice
#pragma once

// global definitions
#define REFRESH_RATE 1000 // refreshrate 1000 ms

// screen definitions
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
#define SCREEN_BACKGROUND TFT_LIGHTGREY

// header definitions
#define HEADER_HEIGHT 40
#define HEADER_BACKGROUND TFT_DARKGREY
#define HEADER_TEXT_X 6
#define HEADER_TEXT_Y 10
#define HEADER_TEXT_COLOR TFT_BLACK

// footer/button definitions
#define FOOTER_HEIGHT 30
#define FOOTER_BACKGROUND TFT_LIGHTGREY

#define BUTTON_COLOR TFT_DARKGREY
#define BUTTON_WIDTH 102
#define BUTTON_HEIGHT 30
#define BUTTON_Y 207
#define BUTTON_1_X 4
#define BUTTON_2_X 109
#define BUTTON_3_X 214

#define BUTTON_TEXT_COLOR TFT_BLACK
#define BUTTON_TEXT_INACTIVE_COLOR TFT_LIGHTGREY
#define BUTTON_TEXT_Y BUTTON_Y + 9
#define BUTTON_1_TEXT_X BUTTON_1_X + 28
#define BUTTON_2_TEXT_X BUTTON_2_X + 22
#define BUTTON_3_TEXT_X BUTTON_3_X + 22

// cnavas definitions
#define CANVAS_X 5
#define CANVAS_Y FOOTER_HEIGHT + 15
#define CANVAS_WIDTH SCREEN_WIDTH - 10
#define CANVAS_HEIGHT SCREEN_HEIGHT - HEADER_HEIGHT - FOOTER_HEIGHT - 10

#define CANVAS_TEXT_COLOR TFT_MAROON
#define CANVAS_BACKGROUND TFT_LIGHTGREY

// global Variables
extern byte showScreen; // Declare the variable: default display mode 0=HOME 1=GRAPH 2=SETUP

enum Screens
{
    HOME,
    GRAPH,
    SETUP
};

void UI_Draw_Header(const char *Header, bool WiFi, bool LAN, bool AP, bool CLOCK, bool BATTERY);
void UI_Draw_Footer(const char *Btn1, const char *Btn2, const char *Btn3, bool btn1, bool btn2, bool btn3);

void UI_handleScreens(int16_t refresh);

void UI_showHome();
void UI_showGraph();
void UI_showSetup();

#endif
