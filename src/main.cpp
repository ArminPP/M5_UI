#define Version "M5_UI v1.0"

/*

Selbstentwickelte UI für den M5 Stack.
Alle anderen Libraries sind zu komplex und benötigen zu viel Speicher.







*/
#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)

#include <M5Stack.h>
#include <UI.h>
#include <Arduino.h>

enum Screens
{
  HOME,
  GRAPH,
  SETUP
};

byte ScreenNr = 0; //default display mode 0=HOME 1=GRAPH 2=SETUP

void setup()
{
  M5.begin();
  M5.Power.begin();
  Serial.begin(115200);

  M5.Lcd.setBrightness(50);
  M5.Lcd.fillScreen(SCREEN_BACKGROUND);
  UI_Draw_Header("M5-SENSOR v1.0", 0, 0, 0, 0, 0);
  UI_Draw_Footer("HOME", "GRAPH", "SETUP", 1, 0, 0);
}

void loop()
{

  M5.update();

  if (M5.BtnA.wasReleased())
  {
    Serial.println("button A");
    ScreenNr = HOME;
    UI_Draw_Footer("HOME", "GRAPH", "SETUP", 1, 0, 0);
    // delete canvas
    M5.Lcd.fillRect(0, 100, SCREEN_WIDTH, 50, SCREEN_BACKGROUND);
  }
  else if (M5.BtnB.wasReleased())
  {
    Serial.println("button B");
    ScreenNr = GRAPH;
    UI_Draw_Footer("HOME", "GRAPH", "SETUP", 0, 1, 0);
    // delete canvas
    M5.Lcd.fillRect(0, 100, SCREEN_WIDTH, 50, SCREEN_BACKGROUND);
  }
  else if (M5.BtnC.wasReleased())
  {
    Serial.println("button C");
    ScreenNr = SETUP;
    UI_Draw_Footer("HOME", "GRAPH", "SETUP", 0, 0, 1);
    // delete canvas
    M5.Lcd.fillRect(0, 100, SCREEN_WIDTH, 50, SCREEN_BACKGROUND);
  }

  // Screen update mit millis()
  if (ScreenNr == HOME)
    UI_showHome();
  if (ScreenNr == GRAPH)
    UI_showGraph();
  if (ScreenNr == SETUP)
    UI_showSetup();
}