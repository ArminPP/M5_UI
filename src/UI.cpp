#include <Arduino.h>
#include <UI.h>
#include <M5Stack.h>
// #include "Free_Fonts.h" // Include the header file attached to this sketch

void UI_Draw_Header(const char *Header, bool WiFi, bool LAN, bool AP, bool CLOCK, bool BATTERY)
{
  M5.Lcd.fillRect(0, 0, SCREEN_WIDTH, HEADER_HEIGTH, HEADER_BACKGROUND);

  M5.Lcd.setTextColor(HEADER_TEXT_COLOR, HEADER_BACKGROUND);
  M5.Lcd.setTextSize(2);
  M5.Lcd.drawString(Header, HEADER_TEXT_X, HEADER_TEXT_Y);
}

void UI_Draw_Footer(const char *Btn1, const char *Btn2, const char *Btn3, bool btn1, bool btn2, bool btn3)
{
  // delete Buttons
  M5.Lcd.fillRect(0, SCREEN_HEIGHT, SCREEN_WIDTH, FOOTER_HEIGTH, FOOTER_BACKGROUND);

  // paint Buttons
  M5.Lcd.fillRoundRect(BUTTON_1_X, BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGTH, 6, BUTTON_COLOR);
  M5.Lcd.fillRoundRect(BUTTON_2_X, BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGTH, 6, BUTTON_COLOR);
  M5.Lcd.fillRoundRect(BUTTON_3_X, BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGTH, 6, BUTTON_COLOR);

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
  M5.Lcd.setTextColor(HEADER_TEXT_COLOR, SCREEN_BACKGROUND); // Bei sich ändernden Texten Hintergrund mitangeben!!!!
  M5.Lcd.setTextSize(3);
  M5.Lcd.drawString("HOME", 50, 110);
}
void UI_showGraph()
{
  M5.Lcd.setCursor(30, 110);
  M5.Lcd.setTextColor(HEADER_TEXT_COLOR, SCREEN_BACKGROUND); // Bei sich ändernden Texten Hintergrund mitangeben!!!!
  M5.Lcd.setTextSize(3);
  M5.Lcd.print("Config ");
  M5.Lcd.println(millis());
}
void UI_showSetup()
{
  M5.Lcd.setCursor(30, 110);
  M5.Lcd.setTextColor(HEADER_TEXT_COLOR, SCREEN_BACKGROUND); // Bei sich ändernden Texten Hintergrund mitangeben!!!!
  M5.Lcd.setTextSize(3);
  M5.Lcd.print("SETUP ");
  M5.Lcd.println(millis());
}