#include <Arduino.h>
#include <UI.h>
#include <M5Stack.h>

// NO FREE FONTS! Flicker when updating :-(
// #include "Free_Fonts.h" // Include the header file attached to this sketch

// global Variables
byte showScreen = 0; // Define the variable: default display mode 0=HOME 1=GRAPH 2=SETUP

// internal Variables
unsigned long previousMillis = 0;
const long interval = 2000;

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
  //M5.Lcd.setFreeFont(FSS9);
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

void UI_handleScreens(int16_t refresh)
{
  if (M5.BtnA.wasReleased())
  {
    Serial.println("button A");
    showScreen = HOME;
    UI_Draw_Footer("HOME", "GRAPH", "SETUP", 1, 0, 0);
    M5.Lcd.fillRect(CANVAS_X, CANVAS_Y, CANVAS_WIDTH, CANVAS_HEIGHT, CANVAS_BACKGROUND); // delete Canvas
    UI_showHome();  // draw on time values for better user feeling (instead of waiting für 1st refresh delay)
  }
  else if (M5.BtnB.wasReleased())
  {
    Serial.println("button B");
    showScreen = GRAPH;
    UI_Draw_Footer("HOME", "GRAPH", "SETUP", 0, 1, 0);
    M5.Lcd.fillRect(CANVAS_X, CANVAS_Y, CANVAS_WIDTH, CANVAS_HEIGHT, CANVAS_BACKGROUND); // delete Canvas
    UI_showGraph();  // draw on time values for better user feeling (instead of waiting für 1st refresh delay)
  }
  else if (M5.BtnC.wasReleased())
  {
    Serial.println("button C");
    showScreen = SETUP;
    UI_Draw_Footer("HOME", "GRAPH", "SETUP", 0, 0, 1);
    M5.Lcd.fillRect(CANVAS_X, CANVAS_Y, CANVAS_WIDTH, CANVAS_HEIGHT, CANVAS_BACKGROUND); // delete Canvas
    // UI_showSetup(); // draw on time values for better user feeling (instead of waiting für 1st refresh delay)
  }

  // Screen update with millis()
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
    UI_showSetup();
  }
}