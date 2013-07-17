#include <SPI.h>
#include <SD.h>
#include <FTOLED.h>
#include <fonts/Droid_Sans_24.h>

/*
 * Countdown on a scrolling screen using a large font
 */

const byte pin_cs = 2;
const byte pin_dc = 3;
const byte pin_reset = 6;

OLED oled(pin_cs, pin_dc, pin_reset);
OLED_TextBox box(oled);

const int COUNTDOWN_FROM = 12;
int counter = COUNTDOWN_FROM;

void setup() {
  Serial.begin(115200);
  oled.begin();
  oled.selectFont(Droid_Sans_24);
  box.setForegroundColour(LIMEGREEN);
}

void loop() {
  Serial.print(counter);
  Serial.println("...");

  box.print("Count ");
  box.print(counter);
  box.println(" . . .");
  counter--;
  delay(1000);

  if(counter == 0) {
    box.println("\nAll Done!");
    delay(2000);
    box.clear();
    counter = 12;
  }
}
