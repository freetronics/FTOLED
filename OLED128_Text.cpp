/* OLED128 Text display and font parsing routines
 *
 * Based on text routines from the DMD LED display modules
 */
#include <OLED128.h>

// Six byte header at beginning of FontCreator font structure in PROGMEM

__attribute__((packed))
struct FontHeader {
  uint16_t size;
  uint8_t fixedWidth;
  uint8_t height;
  uint8_t firstChar;
  uint8_t charCount;
};

void OLED::selectFont(const uint8_t * font)
{
  this->font = (uint16_t) font;
}

int OLED::drawChar(const int x, const int y, const char letter, const Colour colour, const Colour background)
{
  if (x <0 || y < 0 || x >= COLUMNS || y >= ROWS) return -1;

  struct FontHeader header;
  memcpy_PF(&header, this->font, sizeof(FontHeader));

  char c = letter;
  if (c == ' ') {
    int charWide = charWidth(' ');
    this->drawFilledBox(x, y, x + charWide, y + header.height, background);
    return charWide;
  }
  uint8_t width = 0;
  uint8_t bytes = (header.height + 7) / 8; // Number of bytes in a single column

  uint16_t index = 0;

  if (c < header.firstChar || c >= (header.firstChar + header.charCount)) return 0;
  c -= header.firstChar;

  if (header.size == 0) {
    // zero length is flag indicating fixed width font (array does not contain width data entries)
    width = header.fixedWidth;
    index = sizeof(FontHeader) + c * bytes * width;
  } else {
    // variable width font, read width data, to get the index
    for (uint8_t i = 0; i < c; i++) {
      index += pgm_read_byte(this->font + sizeof(FontHeader) + i);
    }
    index = index * bytes + sizeof(FontHeader) + header.charCount;
    width = pgm_read_byte(this->font + sizeof(FontHeader) + c);
  }
  if (x < -width || y < -header.height)
    return width;

  assertCS();
  setColumn(x > 0 ? x : 0,x+width > 127 ? 127 : x+width-1);
  setRow(y > 0 ? y : 0,y+header.height > 127 ? 127 : y+header.height-1);
  setIncrementDirection(REMAP_VERTICAL_INCREMENT);
  setWriteRam();


  /* Characters are stored as follows:
   *
   * Each byte is up to 8 vertical pixels (LSB @ top)
   * Each row of bytes is adjacent
   * For fonts >8 pixels high, the bytes are strided by width
   *
   * ie for a font 16 pixels high and 8 pixels wide:
   *
   * P(0,0) P(0,1) P(1,0) P(1,1) P(2,0) P(2,1) P(3,0) P(3,1) ... etc
   *
   *
   * Things are made more annoying for OLED because we need to write to the display
   * bottom-to-top for each column, so striding backwards instead of forwards...
   */

  for(int8_t ox = 0; ox < width; ox++) {
    if(ox+x >= COLUMNS)
      break;
    int8_t oy = 0;
    for(int8_t byte_y = bytes-1; byte_y >= 0; byte_y--) {
      uint8_t data = pgm_read_byte(this->font + index + ox + byte_y * width);
      int8_t start_bit;
      int8_t end_bit;
      if(bytes == 1) {
        start_bit = header.height-1;
        end_bit = 0;
      }
      else {
        start_bit = 7;
        end_bit = (byte_y < bytes-1) ? 0: 7-((header.height-1)%8);
      }
      for(int8_t bit_y = start_bit; bit_y >= end_bit; bit_y--) {
        if(oy+y < ROWS && ox+x > 0 && oy+y > 0) {
          writeData( (data & 1<<bit_y) ? colour : background);
        }
        oy++;
        if(oy == header.height)
          break;
      }
    }
  }
  releaseCS();
  return width;
}

int OLED::charWidth(const char letter)
{
  char c = letter;
  // Space is often not included in font so use width of 'n'
  if (c == ' ') c = 'n';
  uint8_t width = 0;

  struct FontHeader header;
  memcpy_PF(&header, this->font, sizeof(FontHeader));

  uint16_t index = 0;

  if (c < header.firstChar || c >= (header.firstChar + header.charCount)) {
    return 0;
  }
  c -= header.firstChar;

  if (header.size == 0) {
    // zero length is flag indicating fixed width font (array does not contain width data entries)
    return header.fixedWidth;
  } else {
    // variable width font, read width data for character
    width = pgm_read_byte(this->font + sizeof(FontHeader) + c);
  }
  return width;
}

void OLED::drawString(int x, int y, const char *bChars, byte length, Colour foreground, Colour background)
{
    if (x > COLUMNS || y > ROWS)
	return;

    struct FontHeader header;
    memcpy_PF(&header, this->font, sizeof(FontHeader));

    if (y+header.height<0)
      return;

    int strWidth = 0;
    if(x >= 0)
      this->drawLine(x-1 , y, x-1 , y + header.height, background);

    for (int i = 0; i < length; i++) {
      int charWide = this->drawChar(x+strWidth, y, bChars[i], foreground, background);
      if (charWide > 0) {
        strWidth += charWide ;
        this->drawLine(x + strWidth , y, y + strWidth , y + header.height, background);
        strWidth++;
      } else if (charWide < 0) {
        return;
      }
      if (x + strWidth >= COLUMNS)
        return;
    }
}
