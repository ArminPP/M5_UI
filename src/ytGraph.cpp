/***************************************************************************************
 * A set of ytGraph drawing functions
 * Platform: Arduino / ESP
 *
 *  - supports M5Stack and TFT_eSPI()
 *  - fully scalable
 *  - scroll flicker free
 *  - dynamic x axis
 *  - up to 16 channels
 *  -
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
 * v0.6   adapted for USS (06-Dec-2021)
 ****************************************************************************************/

#include "ytGraph.h"

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

void setupGraph(TFT_eSprite &Graph, TFT_eSprite &xAxis)
{
  Graph.setColorDepth(4);                          // max 16 graph lines with different colors
  Graph.createSprite(SPRITE_WIDTH, SPRITE_HEIGTH); // height = width at M5Stack (landscape mode!)

  Graph.fillSprite(GRAPH_BGRND_COLOR);                                       // Note: Sprite is filled with black when created
  Graph.setScrollRect(0, 0, SPRITE_WIDTH, SPRITE_HEIGTH, GRAPH_BGRND_COLOR); // INFO: to choose a different sroll color than black/white it is mandatory to set a scrollRect !!!!

  xAxis.setColorDepth(4); // could not save some kBytes, with depth(1) only black&white colors are available ?!
  xAxis.createSprite(X_AXIS_WIDTH, X_AXIS_HEIGTH);

  xAxis.fillSprite(GRAPH_AXIS_TEXT_BACKG_COLOR);                                       // Note: Sprite is filled with black when created
  xAxis.setScrollRect(0, 0, X_AXIS_WIDTH, X_AXIS_HEIGTH, GRAPH_AXIS_TEXT_BACKG_COLOR); // INFO: to choose a different sroll color than black/white it is mandatory to set a scrollRect !!!!

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

void showGraph(TFT_eSprite &Graph, TFT_eSprite &xAxis, M5Display &TFT)
{
  ytGraphDrawYaxisFrame(TFT);
  xAxis.pushSprite(X_AXIS_LEFT_X, X_AXIS_UPPER_Y); // no Background color
  Graph.pushSprite(SPRITE_LEFT_X, SPRITE_UPPER_Y); // left upper position
  ytGraphDrawLegend(TFT);
}

void printGraph(TFT_eSprite &Graph, TFT_eSprite &xAxis, GraphValues_t &GV)
{
  int16_t scrollX = round(GRAPH_WIDTH / SAMPLE_COUNT); // scroll one sample to the left

  ox1 -= scrollX; // correction of the last point in graph after scrolling to the left
  ox2 -= scrollX;
  ox3 -= scrollX;
  ox4 -= scrollX;
  ox5 -= scrollX;

  Graph.scroll(-scrollX);
  xAxis.scroll(-scrollX);

  ytGraphDrawDynamicGrid(Graph, xAxis, ox4, LastXGridLinePos, GV.TimeStamp);

  ytGraph(Graph, SAMPLE_COUNT, (int)(GV.BMP280_Temperature + 0.5), LEGEND_1_COLOR, ox1, oy1); // cast from float to int and round correctly
  ytGraph(Graph, SAMPLE_COUNT, (int)(GV.SHT30_Temperature + 0.5), LEGEND_2_COLOR, ox2, oy2);
  ytGraph(Graph, SAMPLE_COUNT, (int)(GV.eCO2 + 0.5), LEGEND_3_COLOR, ox3, oy3);
  ytGraph(Graph, SAMPLE_COUNT, (int)(GV.Humidity + 0.5), LEGEND_4_COLOR, ox4, oy4);
  ytGraph(Graph, SAMPLE_COUNT, (int)(GV.TVOC + 0.5), LEGEND_5_COLOR, ox5, oy5);
}

void ytGraphDrawLegend(M5Display &TFT)
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

// -------------------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------------------

void ytGraphDrawGridXaxis(TFT_eSprite &Graph, TFT_eSprite &xAxis, int16_t &LastXGridLinePos)
{
  double i;
  int16_t gTemp;
  double step;

  LastXGridLinePos = 0;

  xAxis.setTextSize(1);
  xAxis.setTextColor(GRAPH_AXIS_TEXT_COLOR, GRAPH_AXIS_TEXT_BACKG_COLOR);

  step = lround((GRAPH_HEIGHT * GRAPH_Y_DIV) / (abs(GRAPH_Y_AXIS_MIN) + GRAPH_Y_AXIS_MAX)); // Scale y axis according to height of graph
  gTemp = GRAPH_HEIGHT;                                                                     // helper to paint axis
  // draw x axis & x grid lines
  for (i = GRAPH_Y_AXIS_MIN; i <= GRAPH_Y_AXIS_MAX; i += GRAPH_Y_DIV)
  {
    if (i == 0)
    {
      Graph.drawFastHLine(0, gTemp, GRAPH_WIDTH, GRAPH_AXIS_LINE_COLOR);
      Graph.drawFastHLine(0, gTemp + 1, GRAPH_WIDTH, GRAPH_AXIS_LINE_COLOR); // BOLD mode
    }
    else
    {
      Graph.drawFastHLine(0, gTemp, GRAPH_WIDTH, GRAPH_GRID_COLOR);
    }
    gTemp -= step; // calculate new position of grid line
    if (gTemp < 0) // due to round errors at variable 'step', correction to zero
      gTemp = 0;
  }

  step = lround((GRAPH_WIDTH * GRAPH_X_DIV) / (abs(GRAPH_X_AXIS_MIN) + SAMPLE_COUNT)); // scale x axis according to width of graph
  gTemp = 0;
  // helper to paint axis
  // draw y grid lines & x scale numbers
  for (i = GRAPH_X_AXIS_MIN; i <= GRAPH_X_AXIS_MAX; i += GRAPH_X_DIV)
  {
    if (i == 0) // draw zero
    {
    }
    else if (i > (GRAPH_X_AXIS_MAX - GRAPH_X_DIV)) // draw the last grid line
    {
      Graph.drawFastVLine(gTemp, GRAPH_HEIGHT - GRAPH_HEIGHT, GRAPH_HEIGHT, GRAPH_GRID_COLOR);
      LastXGridLinePos = gTemp;
    }
    else
    {
      Graph.drawFastVLine(gTemp, GRAPH_HEIGHT - GRAPH_HEIGHT, GRAPH_HEIGHT, GRAPH_GRID_COLOR);
    }
    // draw xaxis division values
    xAxis.setCursor(gTemp - 5, (X_AXIS_HEIGTH - 8)); // vertically aligned
    xAxis.print("00:00");                            // INFO calculate time from past and print it (eg: now 10:10 -> print 09:10, 09:20, 09:30, ..)
                                                     // INFO LAST TIMESTAMP has to be now!!!!
    gTemp += step;                                   // calculate new position of grid line
  }

  xAxis.pushSprite(X_AXIS_LEFT_X, X_AXIS_UPPER_Y); //, GRAPH_BGRND_COLOR
  Graph.pushSprite(SPRITE_LEFT_X, SPRITE_UPPER_Y); // GRAPH_BGRND_COLOR); // left upper position
}

void ytGraphDrawDynamicGrid(TFT_eSprite &Graph, TFT_eSprite &xAxis, int16_t oox, int16_t &LastGridLineXPos, const char *TimeStamp)
{
  static uint32_t lastXaxisValue = GRAPH_X_AXIS_MAX;

  double i;
  int16_t gTemp; // helper events to draw the dif lines
  double step;   // step is the width between two div lines in px

  step = lround((GRAPH_HEIGHT * GRAPH_Y_DIV) / (abs(GRAPH_Y_AXIS_MIN) + GRAPH_Y_AXIS_MAX)); // Scale y axis according to height of graph
  gTemp = GRAPH_HEIGHT;                                                                     // helper to paint axis
  // draw x axis & x grid lines
  for (i = GRAPH_Y_AXIS_MIN; i <= GRAPH_Y_AXIS_MAX; i += GRAPH_Y_DIV)
  {
    if (i == 0)
    {
      Graph.drawFastHLine(oox, gTemp, SAMPLE_COUNT, GRAPH_AXIS_LINE_COLOR);
      Graph.drawFastHLine(oox, gTemp + 1, SAMPLE_COUNT, GRAPH_AXIS_LINE_COLOR);
    }
    else
    {
      Graph.drawFastHLine(oox, gTemp, SAMPLE_COUNT, GRAPH_GRID_COLOR);
    }
    gTemp -= step; // calculate new position of grid line
    if (gTemp < 0) // due to round errors at variable 'step', correction to zero
      gTemp = 0;
  }

  step = lround((GRAPH_WIDTH * GRAPH_X_DIV) / (abs(GRAPH_X_AXIS_MIN) + GRAPH_X_AXIS_MAX)); // scale x division according to width of graph
  gTemp = round(GRAPH_WIDTH / SAMPLE_COUNT);                                               // scroll one sample to the left
  int16_t correction = (GRAPH_WIDTH - LastGridLineXPos);                                   // correction, if lastXGridline is not on the end of the frame
  static uint16_t xScrollCount = correction;                                               // static is OK, because of only one timeline for all graphs!

  xScrollCount += gTemp;
  if (xScrollCount >= step)
  {
    Graph.drawFastVLine(GRAPH_WIDTH, 0, GRAPH_HEIGHT, GRAPH_GRID_COLOR);
    xScrollCount = correction; // restart with correction value

    // draw xaxis division values
    xAxis.setCursor(GRAPH_WIDTH - 5, (X_AXIS_HEIGTH - 8)); // vertically aligned
    lastXaxisValue += GRAPH_X_DIV;                         // increment with division value
    // xAxis.printf("%02i:%02i", 10, (int)(millis() / 1000) % 100); // INFO: don't paint a value, start with the real Time?!
    xAxis.print(TimeStamp);
  }
}

void ytGraphDrawYaxisFrame(M5Display &d)
{
  double i;
  int16_t gTemp;
  double step;

  d.setTextSize(1);
  // d.setTextColor(GRAPH_AXIS_TEXT_COLOR, GRAPH_BGRND_COLOR);
  d.setTextColor(GRAPH_AXIS_TEXT_COLOR, GRAPH_AXIS_TEXT_BACKG_COLOR);

  // draw main Y axis
  d.drawFastVLine(GRAPH_X_LEFT_POS - 1, GRAPH_Y_BOTTOM_POS - GRAPH_HEIGHT,
                  GRAPH_HEIGHT + 2, GRAPH_AXIS_LINE_COLOR); // drawing from top to bottom
  d.drawFastVLine(GRAPH_X_LEFT_POS - 2, GRAPH_Y_BOTTOM_POS - GRAPH_HEIGHT,
                  GRAPH_HEIGHT + 2, GRAPH_AXIS_LINE_COLOR); // 1-2 px left from grid/sprite

  // draw right frame line
  d.drawFastVLine(GRAPH_X_LEFT_POS + GRAPH_WIDTH + 1, GRAPH_Y_BOTTOM_POS - GRAPH_HEIGHT - 2,
                  GRAPH_HEIGHT + 4, GRAPH_GRID_COLOR); // drawing from top to bottom 1px right from grid/sprite
  d.drawFastVLine(GRAPH_X_LEFT_POS + GRAPH_WIDTH + 2, GRAPH_Y_BOTTOM_POS - GRAPH_HEIGHT - 2,
                  GRAPH_HEIGHT + 4, GRAPH_GRID_COLOR);

  // draw upper frame line
  d.drawFastHLine(GRAPH_X_LEFT_POS - 2, GRAPH_Y_BOTTOM_POS - GRAPH_HEIGHT - 1,
                  GRAPH_WIDTH + 4, GRAPH_GRID_COLOR);
  d.drawFastHLine(GRAPH_X_LEFT_POS - 2, GRAPH_Y_BOTTOM_POS - GRAPH_HEIGHT - 2,
                  GRAPH_WIDTH + 4, GRAPH_GRID_COLOR);

  // draw y scale numbers
  step = lround((GRAPH_HEIGHT * GRAPH_Y_DIV) / (abs(GRAPH_Y_AXIS_MIN) + GRAPH_Y_AXIS_MAX)); // Scale y axis according to height of graph
  gTemp = GRAPH_Y_BOTTOM_POS;                                                               // helper to paint axis
  for (i = GRAPH_Y_AXIS_MIN; i <= GRAPH_Y_AXIS_MAX; i += GRAPH_Y_DIV)
  {
    d.setCursor(GRAPH_X_LEFT_POS - 20, gTemp - 4);
    d.println(i, 0); // only integer

    gTemp -= step; // calculate new position of grid line
    if (gTemp < 0) // due to round errors at variable 'step', correction to zero
      gTemp = 0;
  }
}

void ytGraph(TFT_eSprite &Graph, uint16_t x, int16_t y, uint16_t LineColor, int16_t &ox, int16_t &oy)
{
  y = abs(lround((GRAPH_HEIGHT * y) / (abs(GRAPH_Y_AXIS_MIN) + GRAPH_Y_AXIS_MAX)) - GRAPH_HEIGHT); // calculate y position in sprite and flip
  x = lround((GRAPH_WIDTH * x) / (abs(GRAPH_X_AXIS_MIN) + GRAPH_X_AXIS_MAX));                      // calculate x position in sprite

  Graph.drawLine(ox, oy, x, y, LineColor);
  Graph.drawLine(ox, oy + 1, x, y + 1, LineColor);
  Graph.drawLine(ox, oy - 1, x, y - 1, LineColor);

  ox = x; // store the latest x position for next drawing event
  oy = y; // store the latest y position for next drawing event
}