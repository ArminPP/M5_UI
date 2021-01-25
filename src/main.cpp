#define Version "M5_UI v1.0"

/*

Selbstentwickelte UI für den M5 Stack.
Alle anderen Libraries sind zu komplex und benötigen zu viel Speicher.




*/
#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)

#include <M5Stack.h>
#include <UI.h>
#include <Arduino.h>



// LCD Status
bool LCD = true;

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
  UI_handleScreens(REFRESH_RATE);


// DIM DISPLAY
// https://m5stack.hackster.io/hague/covid-19-data-monitor-dfd267

  if (M5.BtnA.pressedFor(2000))
  {
    if (LCD)
    {
      // M5.Lcd.writecommand(ST7735_DISPOFF);
      M5.Lcd.setBrightness(0);
      LCD = !LCD;
    }
    else
    {
      // M5.Lcd.writecommand(ST7735_DISPON);
      M5.Lcd.setBrightness(100); //Brightness (0: Off - 255: Full)
      LCD = !LCD;
    }
  }
}