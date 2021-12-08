/*
  This example draws fonts (as used by the Adafruit_GFX library) onto the
  screen. These fonts are called the GFX Free Fonts (GFXFF) in this library.

  Other True Type fonts could be converted using the utility within the
  "fontconvert" folder inside the library. This converted has also been
  copied from the Adafruit_GFX library.

  Since these fonts are a recent addition Adafruit do not have a tutorial
  available yet on how to use the utility.   Linux users will no doubt
  figure it out!  In the meantime there are 48 font files to use in sizes
  from 9 point to 24 point, and in normal, bold, and italic or oblique
  styles.

  This example sketch uses both the print class and drawString() functions
  to plot text to the screen.

  Make sure LOAD_GFXFF is defined in the User_Setup.h file within the
  TFT_eSPI library folder.

  --------------------------- NOTE ----------------------------------------
  The free font encoding format does not lend itself easily to plotting
  the background without flicker. For values that changes on screen it is
  better to use Fonts 1- 8 which are encoded specifically for rapid
  drawing with background.
  -------------------------------------------------------------------------

  >>>>>>>>>>>>>>>>>>>>>>>>>>> WARNING <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

  As supplied with the default settings the sketch has 11 fonts loaded,
  i.e. GLCD (Font 1), Font 2, Font 4, Font 6, Font 7, Font 8 and five Free Fonts,
  even though they are not all used in the sketch.

  Disable fonts you do not need in User_Setup.h in the library folder.

  #########################################################################
  ###### DON'T FORGET TO UPDATE THE User_Setup.h FILE IN THE LIBRARY ######
  #########################################################################
*/

#include <M5Stack.h>
#include "Free_Fonts.h" // Include the header file attached to this sketch

void setup(void) {

  M5.begin();
  M5.Power.begin();

  int xpos =  50;
  int ypos = 10;

  Serial.println("Press some serial key or M5 Button B to start program"); // DEBUG
  M5.Lcd.println("\nPress some serial key or M5 Button B to start program");
  while (Serial.available() == 0)
  {
    M5.update();
    if (M5.BtnB.wasPressed())
    { // if M5 Buttons B was pressed, then also start...
      break;
    }
  }

  M5.Lcd.clear(TFT_LIGHTGREY);


  M5.Lcd.setTextColor(TFT_YELLOW, TFT_RED); // Bei sich ändernden Texten Hintergrund mitangeben!!!!
  // Set text padding to 100 pixels wide area to over-write old values on screen
  M5.Lcd.setFreeFont(FS9);       // Select Free Serif 12 point font
  M5.Lcd.setTextPadding(100);

  for (int i = 0; i <= 50; i++) {
    M5.Lcd.drawFloat(i / 10.0, 1, xpos, ypos, GFXFF);
    delay (50);
  }

  // xpos = xpos - 40;
  ypos = ypos + 40;
  M5.Lcd.setTextFont(0);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setTextColor(TFT_YELLOW, TFT_RED); // Bei sich ändernden Texten Hintergrund mitangeben!!!!
  M5.Lcd.setCursor(xpos, ypos);
  for (int i = 0; i <= 50; i++) {
    M5.Lcd.setCursor(xpos, ypos);
    M5.Lcd.printf("T 1=%5.1f÷C", i / 10.0);
    delay (50);
  }

//  M5.Lcd.setTextFont(2);
//  M5.Lcd.setTextSize(2);
//  M5.Lcd.println("\nT 1=10÷C");
//
//  M5.Lcd.setTextFont(4);
//  M5.Lcd.setTextSize(2);
//  M5.Lcd.println("T 1=10÷C");
//
//  M5.Lcd.setTextFont(6);
//  M5.Lcd.setTextSize(2);
//  M5.Lcd.println("T 1=10÷C");
//
//  M5.Lcd.setTextFont(7);
//  M5.Lcd.setTextSize(2);
//  M5.Lcd.println("T 1=10÷C");
//
//  M5.Lcd.setTextFont(8);
//  M5.Lcd.setTextSize(2);
//  M5.Lcd.println("T 1=10÷C");




}

void loop()
{
}



// There follows a crude way of flagging that this example sketch needs fonts which
// have not been enbabled in the User_Setup.h file inside the TFT_HX8357 library.
//
// These lines produce errors during compile time if settings in User_Setup are not correct
//
// The error will be "does not name a type" but ignore this and read the text between ''
// it will indicate which font or feature needs to be enabled
//
// Either delete all the following lines if you do not want warnings, or change the lines
// to suit your sketch modifications.

#ifndef LOAD_GLCD
//ERROR_Please_enable_LOAD_GLCD_in_User_Setup
#endif

#ifndef LOAD_FONT2
//ERROR_Please_enable_LOAD_FONT2_in_User_Setup!
#endif

#ifndef LOAD_FONT4
//ERROR_Please_enable_LOAD_FONT4_in_User_Setup!
#endif

#ifndef LOAD_FONT6
//ERROR_Please_enable_LOAD_FONT6_in_User_Setup!
#endif

#ifndef LOAD_FONT7
//ERROR_Please_enable_LOAD_FONT7_in_User_Setup!
#endif

#ifndef LOAD_FONT8
//ERROR_Please_enable_LOAD_FONT8_in_User_Setup!
#endif

#ifndef LOAD_GFXFF
ERROR_Please_enable_LOAD_GFXFF_in_User_Setup!
#endif
