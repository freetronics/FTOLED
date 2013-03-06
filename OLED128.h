/*

  OLED.h - Support library for Freetronics 128x128 OLED display

  Copyright (C) 2013 Freetronics, Inc. (info <at> freetronics <dot> com)

  Written by Angus Gratton

  Note that this library uses the SPI port for the fastest, low
  overhead writing to the display. Keep an eye on conflicts if there
  are any other devices running from the same SPI port, and that the
  chip select on those devices is correctly set to be inactive when the
  OLED is being written to.

 ---

 This program is free software: you can redistribute it and/or modify it under the terms
 of the version 3 GNU General Public License as published by the Free Software Foundation.

 This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 See the GNU General Public License for more details.

 You should have received a copy of the GNU General Public License along with this program.
 If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef OLED_H_
#define OLED_H_

//Arduino header name is version dependent
#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include "pins_arduino.h"
#include <avr/pgmspace.h>
#include <SPI.h>
#include <SD.h>
#include <digitalWriteFast.h>

#define ROWS 128
#define COLUMNS 128
#define ROW_MASK (ROWS-1)
#define COLUMN_MASK (COLUMNS-1)

struct Colour
{
  byte red   : 5;
  byte green : 6;
  byte blue  : 5;
};

const Colour BLACK = { 0, 0, 0 };
const Colour WHITE = { 31, 63, 31 };
const Colour RED = { 31, 0, 0 };
const Colour GREEN = { 0, 63, 0 };
const Colour BLUE = { 0, 0, 31 };
const Colour YELLOW = {31, 63, 0};
const Colour VIOLET = {31, 0, 31};

const byte MAX_RED = 31;
const byte MAX_GREEN = 63;
const byte MAX_BLUE = 31;
// TODO: add more colours!

// Display clock divisor, pass to setDisplayClock
#define DISPLAY_CLOCK_DIV_1 0
#define DISPLAY_CLOCK_DIV_2 1
#define DISPLAY_CLOCK_DIV_4 2
#define DISPLAY_CLOCK_DIV_8 3
#define DISPLAY_CLOCK_DIV_16 4
#define DISPLAY_CLOCK_DIV_32 5
#define DISPLAY_CLOCK_DIV_64 6
#define DISPLAY_CLOCK_DIV_128 7
#define DISPLAY_CLOCK_DIV_256 8
#define DISPLAY_CLOCK_DIV_512 9
#define DISPLAY_CLOCK_DIV_1024 10

// Remap mode specifiers
#define REMAP_HORIZONTAL_INCREMENT 0
#define REMAP_VERTICAL_INCREMENT (1<<0)

#define REMAP_COLUMNS_LEFT_TO_RIGHT 0
#define REMAP_COLUMNS_RIGHT_TO_LEFT (1<<1)

#define REMAP_ORDER_BGR 0
#define REMAP_ORDER_RGB (1<<2)

#define REMAP_SCAN_UP_TO_DOWN 0
#define REMAP_SCAN_DOWN_TO_UP (1<<4)

#define REMAP_COM_SPLIT_ODD_EVEN (1<<5)

#define REMAP_COLOR_8BIT 0
#define REMAP_COLOR_RGB565 (1<<6)
#define REMAP_COLOR_18BIT (2<<6)

enum OLED_Display_Mode {
  DISPLAY_OFF = 0,                 // No pixels on
  DISPLAY_ALL_PIXELS_FULL = 1,     // All pixels on GS level 63 (ie max brightness)
  DISPLAY_NORMAL = 2,              // Normal display on
  DISPLAY_INVERSE = 3,
};

enum OLED_Command_Lock {
  DISPLAY_COMMAND_UNLOCK = 0x12,        // Allow commands (default state)
  DISPLAY_COMMAND_LOCK = 0x16,          // Disallow all commands until/except next UNLOCK

  DISPLAY_COMMAND_LOCK_SPECIAL = 0xB0,  // Lock out "special" commands always (default state)
  DISPLAY_COMMAND_ALLOW_SPECIAL = 0xB1, // Allow "special" commands when unlocked
};

enum OLED_GPIO_Mode {
  OLED_HIZ = 0,
  OLED_LOW = 2,
  OLED_HIGH = 3,
};

enum BMP_Status {
  BMP_OK = 0,
  BMP_INVALID_FORMAT = 1,
  BMP_UNSUPPORTED_HEADER = 2,
  BMP_TOO_MANY_COLOURS = 3,
  BMP_COMPRESSION_NOT_SUPPORTED = 4,
};

class OLED
{
 public:
  OLED(byte pin_cs, byte pin_dc, byte pin_reset, bool initialise_display);
  OLED(byte pin_cs, byte pin_dc, byte pin_reset) { OLED(pin_cs, pin_dc, pin_reset, true); }

  void initialiseDisplay();

  // Set the colour of a single pixel
  void setPixel(const byte x, const byte y, const Colour colour);

  // Fill the screen with a single solid colour
  void fillScreen(const Colour);
  void clearScreen() { fillScreen(BLACK); }

  // Turn the display on or off
  void setDisplayOn(bool on);

  // Scroll the display by (dX,dY), filling the exposed area with the selected colour
  void scrollDisplay(int dX, int dy, Colour fill_colour);

  //Draw a line from x1,y1 to x2,y2
  void drawLine( int x1, int y1, int x2, int y2, Colour colour );

  //Draw a box(rectangle) from (x1,y1) to (x2,y2), with sides edgeWidth pixels wide
  void drawBox( int x1, int y1, int x2, int y2, int edgeWidth, Colour colour);

  //Draw a filled box(rectangle) from (x1,y1) to (y1,y2), optionally with sides edgeWidth pixels wide
  void drawFilledBox( int x1, int y1, int x2, int y2, Colour fillColour, int edgeWidth, Colour edgeColour);
  void drawFilledBox( int x1, int y1, int x2, int y2, Colour fillColour) { drawFilledBox(x1,y1,x2,y2,fillColour,0,BLACK); }

  // Draw an outline of a circle of radius r at x,y centre, with sides edgeWidth pixels wide
  void drawCircle( int xCenter, int yCenter, int radius, int edgeWidth, Colour colour);

  // Draw an filled circle of radius r at x,y centre, optionally with sides edgeWidth pixels wide
  void drawFilledCircle( int xCenter, int yCenter, int radius, Colour fillColour);

  //Select a text font
  void selectFont(const uint8_t* font);

  //Draw a single character
  int drawChar(const int x, const int y, const char letter, const Colour colour, const Colour background);

  //Find the width of a character
  int charWidth(const char letter);

  // Draw a full string
  void drawString(int x, int y, const char *bChars, byte length, Colour foreground, Colour background);

  //Draw a scrolling string
  void drawMarquee( const char* bChars, byte length, int left, int top);

  // Bitmap stuff

  BMP_Status displayBMP(File &source, const int x, const int y);

  void setGrayscaleTableSystemDefaults();
  void setBrightGrayscaleTable();
  void setDimGrayscaleTable();

  /* Set a custom sized grayscale table. "table" must be address of a
     PROGMEM table holding 64 grayscale level values (GS0..GS63), which must be
     strictly incrementing (see section 8.8 in the datasheet.) Values
     in the table can have values 0-180.
  */
  inline void setGrayscaleTable_P(byte *table);

  // TODO: gpio1 will probably end up being VCC
  void setGPIO(OLED_GPIO_Mode gpio0, OLED_GPIO_Mode gpio1);

 protected:
  byte pin_cs;
  byte pin_dc;
  byte pin_reset;
  byte remap_flags;

  uint16_t font;

  inline void assertCS() { digitalWriteFast(pin_cs, LOW); }
  inline void releaseCS() { digitalWriteFast(pin_cs, HIGH); }

  /* These protected methods are for implementing basic OLED commands.
     They all assume that the CS is asserted before they've been called
  */
  inline void writeCommand(byte command)
  {
    digitalWrite(pin_dc, LOW);
    SPI.transfer(command);
    digitalWrite(pin_dc, HIGH);
  }

  inline void writeData(byte data)
  {
    SPI.transfer(data);
  }

  inline void writeData(Colour colour)
  {
    writeData((colour.green>>3)|(colour.red<<3));
    writeData((colour.green<<5)|(colour.blue));
  }

  inline void writeCommand(byte command, byte data) {
    writeCommand(command);
    writeData(data);
  }

  inline void writeData(byte *data, unsigned int length)
  {
    for(unsigned int i = 0; i < length; i++) {
      writeData(data[i]);
    }
  }

  inline void setColumn(byte start, byte end) {
    writeCommand(0x15);
    writeData(start);
    writeData(end);
  }

  inline void setRow(byte start, byte end) {
    writeCommand(0x75);
    writeData(start);
    writeData(end);
  }

  inline void setWriteRam() {
    writeCommand(0x5C);
  }

// Set the direction to increment
  inline void setIncrementDirection(byte direction)
  {
    byte remap = this->remap_flags & ~(REMAP_HORIZONTAL_INCREMENT|REMAP_VERTICAL_INCREMENT);
    remap = remap | (direction & (REMAP_VERTICAL_INCREMENT|REMAP_HORIZONTAL_INCREMENT));
    writeCommand(0xA0, remap);
    this->remap_flags = remap;
  }

  inline void _setPixel(const byte x, const byte y, const Colour);

  // Direct commands to the module
  inline void setCommandLock(OLED_Command_Lock lock_command)
  {
    writeCommand(0xFD, (byte)lock_command);
  }

  /* set display refresh clock
   * Args:
   * divisor - a constant (DISPLAY_CLOCK_DIV_xxx), see above.
   * frequency - a value 0-15 proportional to oscillator frequency.
   */
  inline void setDisplayClock(byte divisor, byte frequency)
  {
    writeCommand(0xB3, divisor | ((frequency&0x0F)<<4));
  }

  /* set multiplex ratio (Default of ROW_MASK/127 for 128 should be fine in nearly all cases)
   */
  inline void setMultiPlexRatio(byte mux_ratio)
  {
    writeCommand(0xCA, mux_ratio & ROW_MASK);
  }

  /* set display offset row (0-127) */
  inline void setDisplayOffset(byte row)
  {
    writeCommand(0xA2, row & ROW_MASK);
  }

  /* set starting row for display (0-127) */
  inline void setStartRow(byte row)
  {
    writeCommand(0xA1, row & ROW_MASK);
  }

  /* set up address/pixel remap format, see flags REMAP_xxxx_xxxx above */
  inline void setRemapFormat(byte remap_flags)
  {
    writeCommand(0xA0, remap_flags);
    this->remap_flags = remap_flags;
  }

  /* set color channel contrasts. A,B,C are R,G,B values unless REMAP_ORDER_BGR is set */
  inline void setColorContrasts(byte a, byte b, byte c)
  {
    writeCommand(0xC1);
    writeData(a);
    writeData(b);
    writeData(c);
  }

  /* Set Master contrast, value 0-15 */
  inline void setMasterContrast(byte contrast)
  {
    writeCommand(0xC7, contrast & 0x0F);
  }

  /* See the datasheet documentation for the phase length command */
  inline void setPhaseLength(byte phase) {
    writeCommand(0xB1, phase);
  }

  /* Set precharge voltage, level is a proportion of Vcc where 0x00=0.2 0x1F=0.60,
     Default of 0x17 is 0.50 */
  inline void setPrechargeVoltage(byte level) {
    writeCommand(0xBB, level & 0x1F);
  }

  /* Set precharge period as number of DCLK periods 1-15. Default is 8. */
  inline void setPrechargePeriod(byte clocks) {
    clocks = clocks & 0x0F;
    writeCommand(0xB6, clocks ? clocks : 8);
  }

  /* Set display mode. See enum OLED_Display_Mode, above. */
  inline void setDisplayMode(OLED_Display_Mode mode) {
    writeCommand(0xA4+(byte)mode);
  }

  /* Set lock bits. Reset means 0x12, 0x16? means nothing works except reset and another unlock? */
  inline void setLockBits(byte lock_bits) {
    writeCommand(0xFD, lock_bits);
  }
};

#endif
