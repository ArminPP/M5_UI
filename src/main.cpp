#define Version "M5_UI v1.1"
/*

simple menu for (M5Stack) TFT screens 
#####################################

- has a small memory footprint, all other libraries uses to much memory
- only for visualisation purposes (no input possible)



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

  Serial.println("Press some serial key or M5 Button B to start program"); // DEBUG
  M5.Lcd.println("Press some serial key or M5 Button B to start program");
  while (Serial.available() == 0)
  {
    M5.update();
    if (M5.BtnB.wasPressed())
    { // if M5 Buttons B was pressed, then also start...
      break;
    }
  }

  UI_setupTFT();
}

void loop()
{
  UI_doHandleTFT(REFRESH_RATE);
}