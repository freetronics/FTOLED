/*
 * Render frames of an animation from the "frames" directory (on the SD card) to the display
 *
 * The sample frames in the frames/ directory show flames burning, and were originally
 * made by "Spiney"
 * http://opengameart.org/content/animated-sprites-and-textures (thanks Spiney!)
 *
 */
#include <SPI.h>
#include <SD.h>
#include <FTOLED.h>
#include <fonts/SystemFont5x7.h>

const byte pin_cs = 7;
const byte pin_dc = 2;
const byte pin_reset = 3;

const byte pin_sd_cs = 4;

OLED oled(pin_cs, pin_dc, pin_reset);

// Text box is used to display error messages if any frames
// fail to load
OLED_TextBox text(oled);

const int FRAME_COUNT = 64;

#ifdef __AVR__
// On AVR, messages are stored in PROGMEM so don't take up RAM
#define MSG_NOSD F("MicroSD card not found")
#define MSG_SKIP F("Skipping missing frame ")
#else
#define MSG_NOSD "MicroSD card not found"
#define MSG_SKIP "Skipping missing frame"
#endif

void setup()
{
  oled.begin();
  oled.selectFont(System5x7);
  Serial.begin(115200);
  text.setForegroundColour(RED);
  while(!SD.begin(pin_sd_cs)) {
    Serial.println(MSG_NOSD);
    text.println(MSG_NOSD);
    delay(500);
  }
}

void loop()
{
  uint32_t start = millis();
  for(int i = 0; i < FRAME_COUNT; i++) {
    char filename[20];
    snprintf(filename, sizeof(filename), "frames/%03d.bmp", i);
    File frame = SD.open(filename);
    if(!frame) {
      Serial.print(MSG_SKIP);
      Serial.println(filename);
      text.clear();
      text.print(MSG_SKIP);
      text.println(filename);
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

