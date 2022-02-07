#define Version "M5_UI v1.1"
/*

simple menu for (M5Stack) TFT screens
#####################################

- has a small memory footprint, all other libraries uses to much memory
- only for visualisation purposes (no input possible)

*/

#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)

#include <M5Stack.h>
#include <Arduino.h>
// #undef max // BUG  platformio error: macro "min" requires 2 arguments, but only 1 given
// #undef min

#include "Credentials.h"
#include "Log.h"
#include "Display.h"
#include "SignalProcessing.h"

// LCD Status
bool LCD = true;

// TODO TEST                                                                                .
// TODO TEST                                                                                .
char *runtime()
{
  static char buf[6]{'\0'}; // return value must be static char[]

  unsigned long now;
  byte ss, mm, hh;
  now = millis() / 1000;
  hh = now / 3600;
  mm = (now - hh * 3600) / 60;
  ss = (now - hh * 3600) - mm * 60;

  snprintf(buf, sizeof(buf), "%02i:%02i", mm, ss);
  return buf;
}

char *getAllFreeHeap()
{
  static char freeH[80]{}; // returns the formated free heap space
  sprintf(freeH, "Size:%.2fkB Free:%.2fkB Min:%.2fkB Max:%.2fkB",
          ESP.getHeapSize() / 1024.0,
          ESP.getFreeHeap() / 1024.0,
          ESP.getMinFreeHeap() / 1024.0,
          ESP.getMaxAllocHeap() / 1024.0);
  return freeH;
}

void printFreeHeap() // for debugging issues
{
  Serial.println(getAllFreeHeap());
}
// TODO TEST                                                                                .
// TODO TEST                                                                                .

void WaitForUserInput()
{
  // Serial.println("Press some serial key or M5 Button B to start program"); // DEBUG
  //  M5.Lcd.println("Press some serial key or M5 Button B to start program");
  while (Serial.available() == 0)
  {
    M5.update();
    if (M5.BtnB.wasPressed())
    { // if M5 Buttons B was pressed, then also start...
      break;
    }
  }
}

void setup()
{
  M5.begin();
  M5.Power.begin();
  Serial.begin(115200);

  WaitForUserInput();

  UI_setupTFT(); // INFO IMPORTANT MUST CALLED IN MAIN SETUP FIRST !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
}

uint32_t counter = 0;
// bool e, w, a, in, wa, er, cl; // DEBUG
void loop()
{

  UI_doHandleTFT(Credentials::TFT_REFRESH_RATE);

  // feed Terminal
  // #############

  char cstr[16];
  static unsigned long refreshTerminalPM = 0;
  unsigned long refreshTerminalCM = millis();
  if (refreshTerminalCM - refreshTerminalPM >= 1600) // * random(2, 30))
  {
    refreshTerminalPM = refreshTerminalCM;

    ico_AP = random(0, 2);   // DEBUG
    ico_ERR = random(0, 2);  // DEBUG
    ico_WIFI = random(0, 2); // DEBUG
    ico_CLK = true;          // DEBUG

    itoa(counter++, cstr, 10);

    // LOG(Credentials::LOG_DEBUG, _header_ "DEBUG"); // _header_ ==>  "(File:%s Line:%d Function:%s) %s", __FILE__, __LINE__, __func__,
    LOG(Credentials::LOG_DEBUG, "DEBUG"); // _header_ ==>  "(File:%s Line:%d Function:%s) %s", __FILE__, __LINE__, __func__,

    // if (random(100) > 50)
    LOG(Credentials::LOG_WARNING, "1234567890 SuperWarnung");
    // LOG(Credentials::LOG_WARNING, "12345678901234567890123456789012345678901234567890 SuperWarnung");
    // if (random(2090) > 50)

    LOG(Credentials::LOG_ERROR, "%i %s", counter, "       SuperError");
    // if (random(300) > 90)
    // LOG(Credentials::LOG_INFO, "%i,%u 1234567890123456789012345678901234567890123456789012345678901234567890 SuperINFO", counter, millis());

    //  LOG(Credentials::LOG_NONE, getAllFreeHeap());
  }
  static unsigned long refreshInfoPM = 0;
  unsigned long refreshInfoCM = millis();
  if (refreshInfoCM - refreshInfoPM >= 10000)
  {
    refreshInfoPM = refreshInfoCM;
    LOG(Credentials::LOG_INFO, "     %i    INFO", counter);
  }
  // feed Graph
  // ##########
  static unsigned long refreshGraphPM = 0;
  unsigned long refreshGraphCM = millis();
  if (refreshGraphCM - refreshGraphPM >= 3000)
  {
    refreshGraphPM = refreshGraphCM;

    GraphValues_t GV;
    // add to the the end of the buffer the latest sensor reading

    snprintf(GV.TimeStamp, sizeof(GV.TimeStamp), "%s", runtime());
    GV.BMP280_Temperature = lround(random(2, 5));
    GV.SHT30_Temperature = lround(random(8, 12));
    GV.eCO2 = lround(random(15, 20));
    GV.Humidity = random(30, 35);
    GV.TVOC = random(40, 75);

    UI_GraphPrint(GV);
  }

  // feed ENV
  // ##########
  static unsigned long refreshEnvPM = 0;
  unsigned long refreshEnvCM = millis();
  if (refreshEnvCM - refreshEnvPM >= 500)
  {
    refreshEnvPM = refreshEnvCM;

    GraphValues_t GV;
    // add to the the end of the buffer the latest sensor reading

    snprintf(GV.TimeStamp, sizeof(GV.TimeStamp), "%s", runtime());
    GV.BMP280_Temperature = lround(random(2, 5));
    GV.SHT30_Temperature = lround(random(8, 12));
    GV.eCO2 = lround(random(15, 20));
    GV.Humidity = random(30, 35);
    GV.TVOC = random(40, 75);

    UI_EnvPrint(GV);
  }
}
