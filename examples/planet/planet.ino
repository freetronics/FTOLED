/*
 * Render frames of planet to display
 */
#include <OLED.h>
#include <SD.h>

const byte pin_cs = 2;
const byte pin_dc = 3;
const byte pin_reset = 6;

const byte pin_sd_cs = 4;

OLED oled(pin_cs, pin_dc, pin_reset, false);

const int FRAME_COUNT = 60;

void setup()
{
  Serial.begin(115200);
  SD.begin(pin_sd_cs);
  oled.initialiseDisplay();

  Serial.println("Setup complete");
}

void loop()
{
  for(int i = 0; i < FRAME_COUNT; i++) {
    char filename[16];
    snprintf(filename, sizeof(filename), "frames/%04d.bmp", i);
    File frame = SD.open(filename);
    if(!frame) {
      Serial.print("Skipping missing frame ");
      Serial.println(filename);
    }
    else {
      BMP_Status result = oled.displayBMP(frame, 0, 0);
      Serial.print(filename);
      Serial.print(" - ");
      Serial.println(result);
    }
    frame.close();
  }
}

