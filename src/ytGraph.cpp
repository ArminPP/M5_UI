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
****************************************************************************************/

#include "ytGraph.h"



char *calcTime(int32_t t); // internal helper to calculate the relative time on x-axis

void ytGraphDrawGridXaxis(TFT_eSprite &Graph, TFT_eSprite &xAxis, int16_t &LastXGridLinePos)
{
  double i;
  int16_t gTemp;
  double step;

  LastXGridLinePos = 0;

  Graph.fillSprite(GRAPH_BGRND_COLOR);
  xAxis.fillSprite(GRAPH_BGRND_COLOR);
  xAxis.setTextSize(1);
  xAxis.setTextColor(GRAPH_AXIS_TEXT_COLOR, GRAPH_BGRND_COLOR);

  step = lround((GRAPH_HEIGHT * GRAPH_Y_DIV) / (abs(GRAPH_Y_AXIS_MIN) + GRAPH_Y_AXIS_MAX)); // Scale y axis according to height of graph
  gTemp = GRAPH_HEIGHT;                                                                     // helper to paint axis
  // draw x axis & x grid lines
  for (i = GRAPH_Y_AXIS_MIN; i <= GRAPH_Y_AXIS_MAX; i += GRAPH_Y_DIV)
  {
    if (i == 0)
    {
      Graph.drawFastHLine(0, gTemp, GRAPH_WIDTH, GRAPH_AXIS_LINE_COLOR);
      Graph.drawFastHLine(0, gTemp + 1, GRAPH_WIDTH, GRAPH_AXIS_LINE_COLOR);
    }
    else
    {
      Graph.drawFastHLine(0, gTemp, GRAPH_WIDTH, GRAPH_GRID_COLOR);
    }
    gTemp -= step; // calculate new position of grid line
    if (gTemp < 0) // due to round errors at variable 'step', correction to zero
      gTemp = 0;
  }

  step = lround((GRAPH_WIDTH * GRAPH_X_DIV) / (abs(GRAPH_X_AXIS_MIN) + (SAMPLE_COUNT * SAMPLE_RATE))); // scale x axis according to width of graph
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
    xAxis.setCursor(gTemp + 5, (X_AXIS_HEIGTH - 8)); // vertically aligned
    xAxis.print(calcTime((int)i));                   // helper to calculate the relative time on x-axis
    gTemp += step;                                   // calculate new position of grid line
  }

  xAxis.pushSprite(X_AXIS_LEFT_X, X_AXIS_UPPER_Y, GRAPH_BGRND_COLOR); //, GRAPH_BGRND_COLOR
  Graph.pushSprite(SPRITE_LEFT_X, SPRITE_UPPER_Y, GRAPH_BGRND_COLOR); // left upper position
}

void ytGraphDrawDynamicGrid(TFT_eSprite &Graph, TFT_eSprite &xAxis, int16_t oox, int16_t &LastGridLineXPos)
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
    xAxis.setCursor(GRAPH_WIDTH + 5, (X_AXIS_HEIGTH - 8)); // vertically aligned
    lastXaxisValue += GRAPH_X_DIV;                         // increment with division value
    xAxis.print(calcTime(lastXaxisValue));
  }
}

void ytGraphDrawYaxisFrame(M5Display &d)
{
  double i;
  int16_t gTemp;
  double step;

  d.setTextSize(1);
  d.setTextColor(GRAPH_AXIS_TEXT_COLOR, GRAPH_BGRND_COLOR);

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

  // x-axis unit label
  d.setTextColor(GRAPH_AXIS_LINE_COLOR, GRAPH_BGRND_COLOR);
  d.setCursor(GRAPH_X_LEFT_POS + GRAPH_WIDTH + 6, GRAPH_Y_BOTTOM_POS - 5);
  d.println(GRAPH_X_AXIS_LABEL);
  // y-axis unit label
  d.setCursor(2, GRAPH_Y_BOTTOM_POS - GRAPH_HEIGHT - 15);
  d.println(GRAPH_Y_AXIS_LABEL);
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

char *calcTime(int32_t t) // helper to calculate the relative time on x-axis
{
  uint8_t sec = 0, min = 0, hour = 0, day = 0; // to format time value (from nnn sec to dhms...)
  static char str[20]{};                       // returns the formated time value

  switch (SAMPLE_TIME_FORMAT) // time base to format (s->min->h->d ...)
  {
  case 'S': // base is seconds
    sec = t % 60;
    t = (t - sec) / 60;
    min = t % 60;
    t = (t - min) / 60;
    hour = t % 24;
    t = (t - hour) / 24;
    day = t;

    if (day > 0)
      sprintf(str, "%id%ih%im%is", day, hour, min, sec);
    else if (hour > 0)
      sprintf(str, "%ih%im%is", hour, min, sec);
    else if (min > 0)
      sprintf(str, "%im%is", min, sec);
    else if (sec >= 0) // >= to draw the initial zero for the 1st time
      sprintf(str, "%is", sec);
    break;
  case 'M': // base is minutes
    min = t % 60;
    t = (t - min) / 60;
    hour = t % 24;
    t = (t - hour) / 24;
    day = t;

    if (day > 0)
      sprintf(str, "%id%ih%im", day, hour, min);
    else if (hour > 0)
      sprintf(str, "%ih%im", hour, min);
    else if (min >= 0) // >= to draw the initial zero for the 1st time
      sprintf(str, "%im", min);
    break;
  case 'H': // base is hours
    hour = t % 24;
    t = (t - hour) / 24;
    day = t;

    if (day > 0)
      sprintf(str, "%id%ih", day, hour);
    else if (hour >= 0) // >= to draw the initial zero for the 1st time
      sprintf(str, "%ih", hour);
    break;

  default:
    sprintf(str, "%i", t); // add new x axis value - unformatted
    break;
  }

  return str;
}