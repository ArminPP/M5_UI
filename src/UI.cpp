#include <Arduino.h>
#include <UI.h>
#include <M5Stack.h>

// NO FREE FONTS! Flicker when updating :-(
// #include "Free_Fonts.h" // Include the header file attached to this sketch

// global Variables
byte showScreen = 0; // Define the variable: default display mode 0=HOME 1=GRAPH 2=SETUP

// internal Variables
unsigned long previousMillis = 0;
unsigned long previousMillis_LCD = 0;
int8_t timerLCD = 0;

void UI_Draw_Header(const char *Header, bool WiFi, bool LAN, bool AP, bool CLOCK, bool BATTERY)
{
  M5.Lcd.fillRect(0, 0, SCREEN_WIDTH, HEADER_HEIGHT, HEADER_BACKGROUND);

  M5.Lcd.setTextColor(HEADER_TEXT_COLOR, HEADER_BACKGROUND);
  M5.Lcd.setTextSize(2);
  M5.Lcd.drawString(Header, HEADER_TEXT_X, HEADER_TEXT_Y);
}

void UI_Draw_Footer(const char *Btn1, const char *Btn2, const char *Btn3, bool btn1, bool btn2, bool btn3)
{
  // delete Buttons
  M5.Lcd.fillRect(0, SCREEN_HEIGHT, SCREEN_WIDTH, FOOTER_HEIGHT, FOOTER_BACKGROUND);

  // paint Buttons
  M5.Lcd.fillRoundRect(BUTTON_1_X, BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT, 6, BUTTON_COLOR);
  M5.Lcd.fillRoundRect(BUTTON_2_X, BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT, 6, BUTTON_COLOR);
  M5.Lcd.fillRoundRect(BUTTON_3_X, BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT, 6, BUTTON_COLOR);

  M5.Lcd.setTextSize(2);
  // M5.Lcd.setFreeFont(FSS9);
  M5.Lcd.setTextColor(BUTTON_TEXT_COLOR, BUTTON_COLOR);
  if (btn1)
  {
    M5.Lcd.drawString(Btn1, BUTTON_1_TEXT_X, BUTTON_TEXT_Y);
    M5.Lcd.setTextColor(BUTTON_TEXT_INACTIVE_COLOR, BUTTON_COLOR);
    M5.Lcd.drawString(Btn2, BUTTON_2_TEXT_X, BUTTON_TEXT_Y);
    M5.Lcd.drawString(Btn3, BUTTON_3_TEXT_X, BUTTON_TEXT_Y);
  }
  else if (btn2)
  {
    M5.Lcd.drawString(Btn2, BUTTON_2_TEXT_X, BUTTON_TEXT_Y);
    M5.Lcd.setTextColor(BUTTON_TEXT_INACTIVE_COLOR, BUTTON_COLOR);
    M5.Lcd.drawString(Btn1, BUTTON_1_TEXT_X, BUTTON_TEXT_Y);
    M5.Lcd.drawString(Btn3, BUTTON_3_TEXT_X, BUTTON_TEXT_Y);
  }
  else if (btn3)
  {
    M5.Lcd.drawString(Btn3, BUTTON_3_TEXT_X, BUTTON_TEXT_Y);
    M5.Lcd.setTextColor(BUTTON_TEXT_INACTIVE_COLOR, BUTTON_COLOR);
    M5.Lcd.drawString(Btn1, BUTTON_1_TEXT_X, BUTTON_TEXT_Y);
    M5.Lcd.drawString(Btn2, BUTTON_2_TEXT_X, BUTTON_TEXT_Y);
  }
}

// UI_Draw_Footer2(HOME, GRAPH, SETUP, 1, 0, 0);
//
void UI_Draw_Footer2(uint8_t TextA, uint8_t TextB, uint8_t TextC, bool btn1, bool btn2, bool btn3)
{
  char BtnTextA[15] = {'\0'};
  switch (TextA)
  {
  case HOME:
    snprintf(BtnTextA, sizeof(BtnTextA), "%s", "HOME");
    break;
  case GRAPH:
    snprintf(BtnTextA, sizeof(BtnTextA), "%s", "GRAPH");
    break;
  case SETUP:
    snprintf(BtnTextA, sizeof(BtnTextA), "%s", "SETUP");
    break;
  case SCREEN4:
    snprintf(BtnTextA, sizeof(BtnTextA), "%s", "SCREEN4");
    break;
  case SCREEN5:
    snprintf(BtnTextA, sizeof(BtnTextA), "%s", "SCREEN5");
    break;
  }

  // delete Buttons
  M5.Lcd.fillRect(0, SCREEN_HEIGHT, SCREEN_WIDTH, FOOTER_HEIGHT, FOOTER_BACKGROUND);

  // paint Buttons
  M5.Lcd.fillRoundRect(BUTTON_1_X, BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT, 6, BUTTON_COLOR);
  M5.Lcd.fillRoundRect(BUTTON_2_X, BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT, 6, BUTTON_COLOR);
  M5.Lcd.fillRoundRect(BUTTON_3_X, BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT, 6, BUTTON_COLOR);

  M5.Lcd.setTextSize(2);
  // M5.Lcd.setFreeFont(FSS9);
  M5.Lcd.setTextColor(BUTTON_TEXT_COLOR, BUTTON_COLOR);
  if (btn1)
  {
    M5.Lcd.drawString(Btn1, BUTTON_1_TEXT_X, BUTTON_TEXT_Y);
    M5.Lcd.setTextColor(BUTTON_TEXT_INACTIVE_COLOR, BUTTON_COLOR);
    M5.Lcd.drawString(Btn2, BUTTON_2_TEXT_X, BUTTON_TEXT_Y);
    M5.Lcd.drawString(Btn3, BUTTON_3_TEXT_X, BUTTON_TEXT_Y);
  }
  else if (btn2)
  {
    M5.Lcd.drawString(Btn2, BUTTON_2_TEXT_X, BUTTON_TEXT_Y);
    M5.Lcd.setTextColor(BUTTON_TEXT_INACTIVE_COLOR, BUTTON_COLOR);
    M5.Lcd.drawString(Btn1, BUTTON_1_TEXT_X, BUTTON_TEXT_Y);
    M5.Lcd.drawString(Btn3, BUTTON_3_TEXT_X, BUTTON_TEXT_Y);
  }
  else if (btn3)
  {
    M5.Lcd.drawString(Btn3, BUTTON_3_TEXT_X, BUTTON_TEXT_Y);
    M5.Lcd.setTextColor(BUTTON_TEXT_INACTIVE_COLOR, BUTTON_COLOR);
    M5.Lcd.drawString(Btn1, BUTTON_1_TEXT_X, BUTTON_TEXT_Y);
    M5.Lcd.drawString(Btn2, BUTTON_2_TEXT_X, BUTTON_TEXT_Y);
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
  M5.Lcd.drawString("Temperature 1: " + String((millis() % 128) * 0.1) + " C  ", 10, 50);
  M5.Lcd.drawString("Temperature 2: " + String((millis() % 118) * 0.1) + " C  ", 10, 70);
  M5.Lcd.drawString("Temperature 3: " + String((millis() % 108) * 0.1) + " C  ", 10, 90);
  M5.Lcd.drawString("Humidity:      " + String((millis() % 102)) + " %   ", 10, 120);
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
  UI_Draw_Header("M5-SENSOR v1.0", 0, 0, 0, 0, 0); // repaint header, to clear embedded progress bar !!
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
}

void UI_timeoutLCD()
{
  // After timeout (e.g. 30 s) display will get dark

  // 1 sec ticker
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis_LCD >= 1000)
  {
    previousMillis_LCD = currentMillis;
    if (timerLCD <= LCD_TIMEOUT) // because of overflow of int_8
    {
      timerLCD += 1;
      UI_showTimeoutProgressLCD(timerLCD, LCD_TIMEOUT);
    }
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

void UI_handleScreens(int16_t refresh)
{
  if (M5.BtnA.wasReleased())
  {
    Serial.println("button A");
    if (showScreen == 2 || showScreen == 3 || showScreen == 4)
    {
      showScreen = SCREEN4;
      UI_Draw_Footer("< SETUP", "SCREEN4", "SCREEN5 >", 0, 1, 0);
      UI_showScreen4;
    }
    else if (showScreen == 3 || showScreen == 4 || showScreen == 5)
    {
      showScreen = SCREEN4;
      UI_Draw_Footer("< SETUP", "SCREEN4", "SCREEN5 >", 0, 1, 0);
      UI_showScreen4;
    }

    M5.Lcd.fillRect(CANVAS_X, CANVAS_Y, CANVAS_WIDTH, CANVAS_HEIGHT, CANVAS_BACKGROUND); // delete Canvas
    UI_restartTimerLCD();                                                                // draw values immedeatly for better user experience (instead of waiting for the 1st refresh delay)
  }

  else if (M5.BtnB.wasReleased())
  {
    Serial.println("button B");
    showScreen = GRAPH;
    UI_Draw_Footer("HOME", "GRAPH", "SETUP", 0, 1, 0);
    M5.Lcd.fillRect(CANVAS_X, CANVAS_Y, CANVAS_WIDTH, CANVAS_HEIGHT, CANVAS_BACKGROUND); // delete Canvas
    UI_showGraph();                                                                      // draw values immedeatly for better user experience (instead of waiting for the 1st refresh delay)
    UI_restartTimerLCD();
  }
  else if (M5.BtnC.wasReleased())
  {
    Serial.println("button C");

    if (showScreen <= 3)
    {
      showScreen = SETUP;
      UI_Draw_Footer("< GRAPH", "SETUP", "SCREEN4 >", 0, 1, 0);
      UI_showSetup(); // draw values immedeatly for better user experience (instead of waiting for the 1st refresh delay)
    }
    else if (showScreen == 4)
    {
      showScreen = SCREEN4;
      UI_Draw_Footer("< SETUP", "SCREEN4", "SCREEN5 >", 0, 1, 0);
      UI_showScreen4; // draw values immedeatly for better user experience (instead of waiting for the 1st refresh delay)
    }

    M5.Lcd.fillRect(CANVAS_X, CANVAS_Y, CANVAS_WIDTH, CANVAS_HEIGHT, CANVAS_BACKGROUND); // delete Canvas
    UI_restartTimerLCD();
  }

  void UI_handleScreens2(int16_t refresh)
  {
    if (M5.BtnA.wasReleased())
    {
      Serial.println("button A");
      if (showScreen == 2 || showScreen == 3 || showScreen == 4)
      {
        showScreen = SCREEN4;
        UI_Draw_Footer("< SETUP", "SCREEN4", "SCREEN5 >", 0, 1, 0);
        UI_showScreen4;
      }
      else if (showScreen == 3 || showScreen == 4 || showScreen == 5)
      {
        showScreen = SCREEN4;
        UI_Draw_Footer("< SETUP", "SCREEN4", "SCREEN5 >", 0, 1, 0);
        UI_showScreen4;
      }

      M5.Lcd.fillRect(CANVAS_X, CANVAS_Y, CANVAS_WIDTH, CANVAS_HEIGHT, CANVAS_BACKGROUND); // delete Canvas
      UI_restartTimerLCD();                                                                // draw values immedeatly for better user experience (instead of waiting for the 1st refresh delay)
    }

    else if (M5.BtnB.wasReleased())
    {
      Serial.println("button B");
      showScreen = GRAPH;
      UI_Draw_Footer("HOME", "GRAPH", "SETUP", 0, 1, 0);
      M5.Lcd.fillRect(CANVAS_X, CANVAS_Y, CANVAS_WIDTH, CANVAS_HEIGHT, CANVAS_BACKGROUND); // delete Canvas
      UI_showGraph();                                                                      // draw values immedeatly for better user experience (instead of waiting for the 1st refresh delay)
      UI_restartTimerLCD();
    }
    else if (M5.BtnC.wasReleased())
    {
      Serial.println("button C");

      if (showScreen <= 3)
      {
        showScreen = SETUP;
        UI_Draw_Footer("< GRAPH", "SETUP", "SCREEN4 >", 0, 1, 0);
        UI_showSetup(); // draw values immedeatly for better user experience (instead of waiting for the 1st refresh delay)
      }
      else if (showScreen == 4)
      {
        showScreen = SCREEN4;
        UI_Draw_Footer("< SETUP", "SCREEN4", "SCREEN5 >", 0, 1, 0);
        UI_showScreen4; // draw values immedeatly for better user experience (instead of waiting for the 1st refresh delay)
      }

      M5.Lcd.fillRect(CANVAS_X, CANVAS_Y, CANVAS_WIDTH, CANVAS_HEIGHT, CANVAS_BACKGROUND); // delete Canvas
      UI_restartTimerLCD();
    }

    UI_timeoutLCD(); // manage timeout for LCD brightness

    // refresh values on screen - update with millis()
    if (showScreen == HOME)
    {
      unsigned long currentMillis = millis();
      if (currentMillis - previousMillis >= refresh)
      {
        previousMillis = currentMillis;
        UI_showHome();
      }
    }
    else if (showScreen == GRAPH)
    {
      unsigned long currentMillis = millis();
      if (currentMillis - previousMillis >= refresh)
      {
        previousMillis = currentMillis;
        UI_showGraph();
      }
    }
    else if (showScreen == SETUP)
    {
      UI_showSetup(); // shows screen with no refresh rate ! (updtes with loop() speed!)
    }
  }
