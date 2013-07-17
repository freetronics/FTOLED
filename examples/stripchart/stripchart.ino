/*
 * Plot a chart reading of analog input values in realtime, with the
 * raw numeric values written in text below
 */

const byte pin_cs = 2;
const byte pin_dc = 3;
const byte pin_reset = 6;

#include <SPI.h>
#include <SD.h>
#include "FTOLED.h"
#include "fonts/SystemFont5x7.h"

OLED oled(pin_cs, pin_dc, pin_reset);
// Text box takes up bottom 32 characters of the display (ie 4 rows)
OLED_TextBox box(oled, 0, 0, 128, 32);

const int NUM_CHANNELS = 3;

const Colour COLOURS[NUM_CHANNELS] = { RED, GREEN, BLUE };

// How many ms between each sample (the sample rate will be a bit slower than this, due to overhead of actually drawing things.)
const int DELAY = 50;

int samples[NUM_CHANNELS][128];
uint8_t cur_sample = 0;
uint8_t last_y[NUM_CHANNELS];

void setup() {
  oled.begin();
  oled.selectFont(System5x7);
  box.setForegroundColour(DARKGREEN);
}

void loop() {
  if(cur_sample%2 == 0)
    box.reset();
  box.print(millis());
  box.println(":");
  for(int ch = 0; ch < NUM_CHANNELS; ch++) {
    // Note that as we're switching channels a lot here we might need to
    // do some clever things if reading high-impedance sources
    samples[ch][cur_sample] = analogRead(ch);
    box.print(" ");
    box.setForegroundColour(COLOURS[ch]);
    box.print(ch);
    box.print(": ");
    box.setForegroundColour(DARKGREEN);
    box.print(samples[ch][cur_sample]);
  }
  // The extra spaces are to avoid artifacts when updating, the alternative would
  // be to call box.clear() instead of box.reset(), but then the display flickers.
  box.println("  ");

  for(int ch = 0; ch < NUM_CHANNELS; ch++) {
    uint8_t y = map(samples[ch][cur_sample], 0, 1023, 33, 128);
    if(cur_sample > 0)
      oled.drawLine(cur_sample-1,last_y[ch],cur_sample,y, COLOURS[ch]);
    else
      oled.setPixel(cur_sample, y, COLOURS[ch]);
    last_y[ch] = y;
  }

  delay(DELAY);
  cur_sample++;
  if(cur_sample > 127) {
    oled.drawFilledBox(0,33,128,128,BLACK);
    cur_sample = 0;
  }
}
