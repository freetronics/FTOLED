#include <OLED.h>

// Swap A & B "in place" (well, with a temp variable!)
template<typename T> inline void swap(T &a, T &b)
{
  T tmp(a); a=b; b=tmp;
}

// Check a<=b, and swap them otherwise
template<typename T> inline void ensureOrder(T &a, T &b)
{
  if(b<a) swap(a,b);
}

// I don't know why, but although spi.transfer() is declared
// inline it won't inline, but this method will... they're both available at link time?!?
static inline byte _spi_transfer(byte _data) {
  SPDR = _data;
  while (!(SPSR & _BV(SPIF)))
    ;
  return SPDR;
}

OLED::OLED(byte pin_cs, byte pin_dc, byte pin_reset, bool initialise_display) :
  pin_cs(pin_cs),
  pin_dc(pin_dc),
  pin_reset(pin_reset)
{
  // Initialise SPI port
  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE3);
  SPI.setClockDivider(SPI_CLOCK_DIV2);

  pinMode(pin_cs, OUTPUT);
  digitalWrite(pin_cs, HIGH);
  pinMode(pin_dc, OUTPUT);
  digitalWrite(pin_dc, HIGH);
  pinMode(pin_reset, OUTPUT);
  digitalWrite(pin_reset, HIGH);

  if(initialise_display)
    initialiseDisplay();
}

void OLED::initialiseDisplay() {
  digitalWrite(pin_reset, HIGH);
  delay(1);
  digitalWrite(pin_reset, LOW);
  delay(1);
  digitalWrite(pin_reset, HIGH);
  delay(1);

  setDisplayOn(false);

  assertCS();

  setLockBits(0x12); // enter unlocked state
  setLockBits(0xB1); // allow all commands

  setDisplayClock(DISPLAY_CLOCK_DIV_1, 15); // "approx 90fps" ?
  setStartRow(32);
  setRemapFormat(REMAP_HORIZONTAL_INCREMENT
                 | REMAP_COLUMNS_LEFT_TO_RIGHT
                 | REMAP_ORDER_RGB
                 | REMAP_SCAN_UP_TO_DOWN
                 | REMAP_COM_SPLIT_ODD_EVEN
                 | REMAP_COLOR_RGB565);

  setColorContrasts(0xC8,0x80,0xC8);
  setMasterContrast(0x0F);
  setPhaseLength(0x32);			// Set Phase 1 as 5 Clocks & Phase 2 as 3 Clocks
  
  releaseCS();

  setGrayscaleTableSystemDefaults();

  assertCS();

  // // // TODO: Work out what this is, command B2h undocumented in datasheet
  // // //Set_Display_Enhancement(0xA4);		// Enhance Display Performance "Enhance Driving Scheme Capability"
  // // writeCommand(0xB2);			// Display Enhancement
  // // writeData(0xA4);				//   Default => 0x00 (Normal)
  // // writeData(0x00);
  // // writeData(0x00);

  // // setPrechargeVoltage(0x17);		// Default precharge voltage, VCC/2
  // // setPrechargePeriod(1); // Commented as this seems short, think maybe it goes w/ prev command

  // // // Set VCOMH
  // // writeCommand(0xBE, 0x05);

  // // setDisplayMode(DISPLAY_NORMAL);

  setMultiPlexRatio(0x7F);		  // 1/128 Duty (0x0F~0x7F)

  releaseCS();

  // these are public methods so they manage CS on their own:
  clearScreen(); 
  setDisplayOn(true);
}

// setPixel has two methods, public method asserts/deasserts CS protected method doesn't
void OLED::setPixel(const byte x, const byte y, const Colour colour)
{
  digitalWrite(pin_cs, LOW);
  _setPixel(x,y,colour);
  digitalWrite(pin_cs, HIGH);
}

inline void OLED::_setPixel(const byte x, const byte y, const Colour colour)
{
  setColumn(x,x);
  setRow(y,y);
  setWriteRam();
  writeData(colour);
}

void OLED::setDisplayOn(bool on)
{
  // temporary VCC controls: TODO, make a GPIO from the module itself
  const byte pin_vcc = 5; // TODO: make this a GPIO from the module itself
  pinMode(pin_vcc, OUTPUT);

  if(on) {
    digitalWrite(pin_vcc, HIGH);
    delay(100);
  }
  assertCS();
  writeCommand(on ? 0xAF : 0xAE);
  releaseCS();

  if(!on) {
    digitalWrite(pin_vcc, LOW);
    delay(100);
  }
}

void OLED::fillScreen(const Colour colour)
{
  digitalWrite(pin_cs, LOW);
  setColumn(0,COLUMN_MASK);
  setRow(0, ROW_MASK);
  setWriteRam();

  for(int p = 0; p < ROWS*COLUMNS; p++) {
    _spi_transfer((colour.green>>3)|(colour.red<<3));
    _spi_transfer((colour.green<<5)|(colour.blue));
  }
  digitalWrite(pin_cs, HIGH);
}

void OLED::scrollDisplay(int dX, int dy, Colour fill_colour)
{
}

void OLED::drawLine( int x1, int y1, int x2, int y2, Colour colour )
{
  // Shortcuts for horizontal and vertical lines, many fewer writes
  assertCS();
  if(x1==x2) {
    setColumn(x1,x2);
    ensureOrder(y1,y2);
    setRow(y1,y2);
    setIncrementDirection(REMAP_VERTICAL_INCREMENT);
    setWriteRam();
    while(y1 <= y2) {
      writeData(colour);
      y1++;
    }
  }
  else if(y1==y2) {
    ensureOrder(x1,x2);
    setColumn(x1,x2);
    setRow(y1,y2);
    setIncrementDirection(REMAP_HORIZONTAL_INCREMENT);
    setWriteRam();
    while(x1 <= x2) {
      writeData(colour);
      x1++;
    }
  }
  else { // Diagonal line, uses code from DMD
    int dy = y2 - y1;
    int dx = x2 - x1;
    int stepx, stepy;

    if (dy < 0) {
	    dy = -dy;
	    stepy = -1;
    } else {
	    stepy = 1;
    }
    if (dx < 0) {
	    dx = -dx;
	    stepx = -1;
    } else {
	    stepx = 1;
    }
    dy <<= 1;			// dy is now 2*dy
    dx <<= 1;			// dx is now 2*dx

    _setPixel(x1, y1, colour);
    if (dx > dy) {
	    int fraction = dy - (dx >> 1);	// same as 2*dy - dx
	    while (x1 != x2) {
	        if (fraction >= 0) {
		        y1 += stepy;
		        fraction -= dx;	// same as fraction -= 2*dx
	        }
	        x1 += stepx;
	        fraction += dy;	// same as fraction -= 2*dy
	        _setPixel(x1, y1, colour);
	    }
    } else {
	    int fraction = dx - (dy >> 1);
	    while (y1 != y2) {
	        if (fraction >= 0) {
		        x1 += stepx;
		        fraction -= dy;
	        }
	        y1 += stepy;
	        fraction += dx;
	        _setPixel(x1, y1, colour);
	    }
    }
  }
  releaseCS();
}

void OLED::drawBox( int x1, int y1, int x2, int y2, int edgeWidth, Colour colour)
{
  // Make sure (x1,y1) is always the top left corner
  ensureOrder(x1,x2);
  ensureOrder(y1,y2);

  assertCS();

  // Left side
  setColumn(x1,x1+edgeWidth-1);
  setRow(y1,y2);
  setWriteRam();
  for(int n=0;n<(1+y2-y1)*edgeWidth;n++) {
    writeData(colour);
  }

  // Top side
  setColumn(x1,x2);
  setRow(y1,y1+edgeWidth-1);
  setWriteRam();
  for(int n=0;n<(1+x2-x1)*edgeWidth;n++) {
    writeData(colour);
  }

  // Right side
  setColumn(x2-(edgeWidth-1),x2);
  setRow(y1,y2);
  setWriteRam();
  for(int n=0;n<(1+y2-y1)*edgeWidth;n++) {
    writeData(colour);
  }

  // Bottom side
  setColumn(x1,x2);
  setRow(y2-(edgeWidth-1),y2);
  setWriteRam();
  for(int n=0;n<(1+y2-y1)*edgeWidth;n++) {
    writeData(colour);
  }

  releaseCS();
}

void OLED::drawFilledBox( int x1, int y1, int x2, int y2, Colour fillColour, int edgeWidth, Colour edgeColour)
{
  // Make sure (x1,y1) is always the top left corner
  ensureOrder(x1,x2);
  ensureOrder(y1,y2);

  assertCS();

  setColumn(x1,x2);
  setRow(y1,y2);
  setIncrementDirection(REMAP_VERTICAL_INCREMENT);
  setWriteRam();

  for(int x = x1; x <= x2; x++) {
    for(int y = y1; y <= y2; y++) {
      if(edgeWidth > 0 && (x<x1+edgeWidth || y<y1+edgeWidth || x>x2-edgeWidth || y>y2-edgeWidth))
        writeData(edgeColour);
      else
        writeData(fillColour);
    }
  }

  releaseCS();
}

void OLED::drawCircle( int xCenter, int yCenter, int radius, int edgeWidth, Colour colour)
{
  // Bresenham's circle drawing algorithm
  int x = -radius;
  int y = 0;
  int error = 2-2*radius;
  assertCS();
  while(x < 0) {
    _setPixel(xCenter-x, yCenter+y, colour);
    _setPixel(xCenter-y, yCenter-x, colour);
    _setPixel(xCenter+x, yCenter-y, colour);
    _setPixel(xCenter+y, yCenter+x, colour);
    radius = error;
    if (radius <= y) error += ++y*2+1;
    if (radius > x || error > y) error += ++x*2+1;
  }
  releaseCS();
}

void OLED::drawFilledCircle( int xCenter, int yCenter, int radius, Colour fillColour)
{
  // Bresenham's circle drawing algorithm, filling with vertical line segments to/from origin
  int x = -radius;
  int y = 0;
  int error = 2-2*radius;
  while(x < 0) {
    drawLine(xCenter-x, yCenter,   xCenter-x, yCenter+y, fillColour);
    drawLine(xCenter-y, yCenter,   xCenter-y, yCenter-x, fillColour);
    drawLine(xCenter+x, yCenter-y, xCenter+x, yCenter, fillColour);
    drawLine(xCenter+y, yCenter+x, xCenter+y, yCenter, fillColour);
    radius = error;
    if (radius <= y) error += ++y*2+1;
    if (radius > x || error > y) error += ++x*2+1;
  }
}

void OLED::setGrayscaleTableSystemDefaults()
{
  assertCS();
  writeCommand(0xB9);
  releaseCS();
}

void OLED::setBrightGrayscaleTable()
{
  byte PROGMEM table[64] = { 0, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11,
                             12, 13, 14, 15, 16, 17, 18, 19, 21,
                             23, 25, 27, 29, 31, 33, 35, 37, 39,
                             42, 45, 48, 51, 54, 57, 60, 63, 66,
                             69, 72, 76, 80, 84, 88, 92, 96, 100,
                             104, 108, 112, 116, 120, 125, 130,
                             135, 140, 145, 150, 155, 160, 165,
                             170, 175, 180 };
  setGrayscaleTable_P(table);
}

void OLED::setDimGrayscaleTable()
{
  byte PROGMEM table[64] = {0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6,
                            6, 7, 7, 8, 8, 9, 9, 10, 11, 12, 13,
                            14, 15, 16, 17, 18, 19, 21, 22, 24,
                            25, 27, 28, 30, 31, 33, 34, 36, 38,
                            40, 42, 44, 46, 48, 50, 52, 54, 56,
                            58, 60, 62, 65, 67, 70, 72, 75, 77,
                            80, 82, 85, 87, 90 };
  setGrayscaleTable_P(table);
}

void OLED::setGrayscaleTable_P(byte *table)
{
  writeCommand(0xB8);
  for(int gs = 0; gs < 63; gs++)
    writeData(pgm_read_byte_near(table+gs));
}
