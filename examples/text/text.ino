#include <OLED128.h>
#include <fonts/SystemFont5x7.h>
#include <fonts/Arial14.h>
#include <fonts/Arial_black_16.h>
#include <fonts/Droid_Sans_36.h>

// NB: This sketch won't fit on older Arduino Duemilanoves or any other
// Arduino with less than 32k of onboard flash storage.
//
// If the sketch doesn't fit, disable one of the fonts by deleting one of
// the blocks below

const byte pin_cs = 2;
const byte pin_dc = 3;
const byte pin_reset = 6;

OLED oled(pin_cs, pin_dc, pin_reset, false);

const char *ARIAL_BLACK_16 = "Arial Black 16";
const char *ARIAL_14 = "Arial 14";
const char *SYSTEM_5_7 = "System 5x7";

void setup() {
  Serial.begin(115200);
  Serial.print("init...");
  oled.initialiseDisplay();

  oled.selectFont(Arial_14);
  oled.drawString(10,16,ARIAL_14,strlen(ARIAL_14),VIOLET,BLACK);

  oled.selectFont(Arial_Black_16);
  oled.drawString(10,32,ARIAL_BLACK_16,strlen(ARIAL_BLACK_16),WHITE,BLACK);

  oled.selectFont(System5x7);
  oled.drawString(10,54,SYSTEM_5_7,strlen(SYSTEM_5_7),RED,BLACK);

  oled.selectFont(Droid_Sans_36);
  oled.drawString(10,72,"DS 36",strlen("DS 36"),GREEN,BLACK);
}

void loop() {
  return;
  for(int i = 0; i < 128; i++) {
    oled.drawString(i,i,"Hello World",strlen("Hello World"),WHITE,BLACK);
    oled.drawLine(i,i,i+80,i,BLACK);
  }
}
