/*
 * Render frames of an animation from the "frames" directory (on the SD card) to the display
 *
 * The sample frames in the frames/ directory show flames burning, and were originally
 * made by "Spiney"
 * http://opengameart.org/content/animated-sprites-and-textures (thanks Spiney!)
 *
 */
#include <OLED128.h>
#include <SD.h>

const byte pin_cs = 2;
const byte pin_dc = 3;
const byte pin_reset = 6;

const byte pin_sd_cs = 4;

OLED oled(pin_cs, pin_dc, pin_reset, false);

const int FRAME_COUNT = 64;

void setup()
{
  Serial.begin(115200);
  SD.begin(pin_sd_cs);
  oled.initialiseDisplay();
}

void loop()
{
  uint32_t start = millis();
  for(int i = 0; i < FRAME_COUNT; i++) {
    char filename[20];
    snprintf(filename, sizeof(filename), "frames/%03d.bmp", i);
    File frame = SD.open(filename);
    if(!frame) {
      Serial.print("Skipping missing frame ");
      Serial.println(filename);
    }
    else {
      BMP_Status result = oled.displayBMP(frame, 0, 0);
    }
    frame.close();
  }
  uint32_t end = millis();
  Serial.print(FRAME_COUNT);
  Serial.print(" frames in ");
  Serial.println(end - start);
}

