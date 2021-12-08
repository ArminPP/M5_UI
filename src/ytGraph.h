/***************************************************************************************
 * A small demo for the ytGraph function set
 * Platform: Arduino / ESP
 *
 * Armin Pressler 2021
 * https://github.com/ArminPP/ytGraph
 *
 *
 *
 * MIT License
 *
 * Changelog:
 * v0.5   initial version (28-Feb-2021)
 * v0.6   adapted for USS (06-Dec-2021) eg. background color is now working :-)
 ****************************************************************************************/

#ifndef YTGRAPH_h
#define YTGRAPH_h

#define useM5STACK // choose between M5Stack and TFT_eSPI

#ifdef useM5STACK     // compiles with the M5Stack variant of TFT_eSPI () library
#include <M5Stack.h>  //
#else                 // and the original library as well
#include <TFT_eSPI.h> //
#endif

#include "SignalProcessing.h"

// --------------------------------------------------------------------------------------

#define LEGEND_1 "Temp1|÷C" // check max. chars per line!
#define LEGEND_2 "Temp2|÷C"
#define LEGEND_3 "Hum|%   "
#define LEGEND_4 "Co|ppm  "
#define LEGEND_5 "TVOC|pp "

#define LEGEND_1_COLOR TFT_CYAN
#define LEGEND_2_COLOR TFT_PINK
#define LEGEND_3_COLOR TFT_YELLOW
#define LEGEND_4_COLOR TFT_MAGENTA
#define LEGEND_5_COLOR 0xFA08 // redish

#define SAMPLE_COUNT 120 // how many samples in one graph

#define GRAPH_X_LEFT_POS 35             // uint - lower left x position
#define GRAPH_Y_BOTTOM_POS 180          // uint - lower left y position MUST BE LARGER THAN GRAPH_HEIGHT!
#define GRAPH_WIDTH 250                 // uint - only axis to axis, without axis/div description in sprite frame
#define GRAPH_HEIGHT 100                // uint - only axis to axis, without axis/div description in sprite frame
#define GRAPH_X_AXIS_MIN 0              // int  - should be >= 0 (Time is positive!)
#define GRAPH_X_AXIS_MAX (SAMPLE_COUNT) // end of axis is (num of samples * sample rate)
#define GRAPH_X_DIV 20                  // uint - division of x axis (time) // INFO not to small, because timestamp needs space "10:34"
#define GRAPH_Y_AXIS_MIN 0.0            // double
#define GRAPH_Y_AXIS_MAX 120.0          // double
#define GRAPH_Y_DIV 30                  // uint - division of y axis

#define SPRITE_LEFT_X GRAPH_X_LEFT_POS                     // this is the "real" position of the graph
#define SPRITE_UPPER_Y (GRAPH_Y_BOTTOM_POS - GRAPH_HEIGHT) // this is the "real" position of the graph
#define SPRITE_WIDTH GRAPH_WIDTH + 1                       // x Grid
#define SPRITE_HEIGTH GRAPH_HEIGHT + 2                     // y Grid & x-axis

#define X_AXIS_WIDTH SPRITE_WIDTH + 30 // Axis width is larger than sprite
#define X_AXIS_HEIGTH 15
#define X_AXIS_LEFT_X GRAPH_X_LEFT_POS - 10
#define X_AXIS_UPPER_Y GRAPH_Y_BOTTOM_POS + 4

#define GRAPH_AXIS_LINE_COLOR TFT_BLACK
#define GRAPH_BGRND_COLOR 0x10A2 // TFT_BLACK //0x630C        // 0x4208 // 0x5ACB // TFT_DARKGREY
#define GRAPH_AXIS_TEXT_COLOR TFT_BLACK
#define GRAPH_AXIS_TEXT_BACKG_COLOR 0xBDD7
#define GRAPH_GRID_COLOR TFT_LIGHTGREY

// --------------------------------------------------------------------------------------

void setupGraph(TFT_eSprite &Graph, TFT_eSprite &xAxis);
void showGraph(TFT_eSprite &Graph, TFT_eSprite &xAxis, M5Display &TFT);
void printGraph(TFT_eSprite &Graph, TFT_eSprite &xAxis, GraphValues_t &GV);

void ytGraphDrawLegend(M5Display &TFT);
void ytGraphDrawYaxisFrame(M5Display &d);
void ytGraphDrawGridXaxis(TFT_eSprite &Graph, TFT_eSprite &xAxis, int16_t &LastXGridLinePos);
void ytGraphDrawDynamicGrid(TFT_eSprite &Graph, TFT_eSprite &xAxis, int16_t oox, int16_t &LastXGridLinePos, const char *TimeStamp);
void ytGraph(TFT_eSprite &Graph, uint16_t x, int16_t y, uint16_t LineColor, int16_t &ox, int16_t &oy);

#endif
