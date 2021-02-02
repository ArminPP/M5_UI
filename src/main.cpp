#define Version "M5_UI v1.1"
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

  M5.Lcd.setBrightness(LCD_BRIGHTNESS); // set default brightness
  M5.Lcd.fillScreen(SCREEN_BACKGROUND);
  UI_Draw_Header(HEADER_TITLE, 0, 0, 0, 0, 0);
  UI_Draw_Footer(BUTTON_A_CAPTION, BUTTON_B_CAPTION, BUTTON_C_CAPTION, 1, 0, 0);
  UI_showTimeoutProgressLCD(0,LCD_TIMEOUT);
}

void loop()
{
  M5.update();
  UI_handleScreens(REFRESH_RATE);
}