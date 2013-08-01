#include <SPI.h>
#include <SD.h>
#include <OLED.h>

const byte pin_cs = 7;
const byte pin_dc = 2;
const byte pin_reset = 4;

OLED oled(pin_cs, pin_dc, pin_reset, false);

void setup() {
  Serial.begin(115200);
  Serial.print("init...");
  oled.initialiseDisplay();
}

void loop() {
  oled.fillScreen(RED);
  delay(1000);
  oled.fillScreen(GREEN);
  delay(1000);
  oled.fillScreen(BLUE);
  delay(1000);
}
