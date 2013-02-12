/* OLED BMP parsing and bitmap display routines for BMP files */

#include <stdint.h>

#include <OLED.h>

// Read a little-endian short word from a stream
inline uint16_t readShort(Stream &s)
{
  return (uint16_t)s.read() | (uint16_t)s.read() << 8;
}

// Read a little-endian long from a stream
inline uint32_t readLong(Stream &s)
{
  return (uint32_t)readShort(s) | (uint32_t)readShort(s) << 16;
}

enum BMP_Compression {
  BMP_NoCompression = 0,
  BMP_RLE8bpp = 1,
  BMP_RLE4bpp = 2,
  // ... lots more methods that aren't supported :)
};

const uint8_t OFFS_DIB_HEADER = 0x0e;

BMP_Status OLED::displayBMP(File &f, const int x, const int y)
{
  f.seek(0);

  // File header, check magic number 'BM'
  if(f.read() != 'B' || f.read() != 'M')
    return BMP_INVALID_FORMAT;

  // Read DIB header with image properties
  f.seek(OFFS_DIB_HEADER);
  uint16_t dib_headersize = readLong(f);
  uint16_t width, height, bpp, compression;

  if(dib_headersize == 12) { // OS/2 BITMAPCOREHEADER format (aka BMPv2)
    width = readShort(f);
    height = readShort(f);
    if(readShort(f) != 1)
      return BMP_UNSUPPORTED_HEADER;
    bpp = readShort(f);
    compression = BMP_NoCompression;
  } else {
    return BMP_UNSUPPORTED_HEADER; // TODO: Support 40-byte modern BMP headers
  }

  Serial.println(bpp);

  // Verify image properties from header
  if(bpp > 16)
    return BMP_TOO_MANY_COLOURS;
  if(bpp != 1 && bpp != 4 && bpp != 8 && bpp != 16)
    return BMP_INVALID_FORMAT;

  if(compression != (int)BMP_NoCompression) {
    //     && !(compression == BMP_RLE8bpp && bpp == 8)
    //     && !(compression == BMP_RLE4bpp && bpp == 4)) {
    return BMP_COMPRESSION_NOT_SUPPORTED;
  }

  // Find the starting offset for the data in the first row
  f.seek(0x0a);
  uint32_t data_offs = readLong(f);

  assertCS();
  // Trim height to 128, anything bigger gets cut off, then set up row span in memory
  if(y + height > 128)
    height = 128-y;
  setRow(y,y+height-1);
  // Calculate outputtable width and set up column span in memory
  uint16_t out_width = width;
  if(x + out_width > 128)
    out_width = 128-x;
  setColumn(x,x+out_width-1);

  // Calculate the width in bits of each row (rounded up to nearest byte)
  uint16_t row_bits = (width*bpp + 7) & ~7;
  // Calculate width in bytes (4-byte boundary aligned)
  uint16_t row_bytes = (row_bits/8 + 3) & ~3;

  setIncrementDirection(REMAP_HORIZONTAL_INCREMENT);
  setWriteRam();
  releaseCS();

  // Read colour palette to RAM. It's quite hefty to hold in RAM (512 bytes for a full 8-bit palette)
  // but don't have much choice as seeking back and forth on SD is painfully slow
  Colour *palette;
  if(bpp != 16) {
    uint8_t palette_size = 1<<(bpp-1);
    palette = (Colour *)malloc(sizeof(Colour)*palette_size);
    f.seek(OFFS_DIB_HEADER + dib_headersize);
    Serial.println(bpp);
    for(int i = 0; i < palette_size; i++) {
      palette[i].blue = f.read() >> 3;
      palette[i].green = f.read() >> 2;
      palette[i].red = f.read() >> 3;
    }
  }

  for(byte row = 0; row < height; row++) {
    f.seek(data_offs + row*row_bytes);
    uint8_t latest; // temp variable for storing latest pixel index byte (used for 4-bit ops)
    for(uint16_t col = 0; col < out_width; col++) {
      Colour pixel;
      if(bpp == 16) { // pixels stored as BGR555 in BMP, read as RGB565
        uint16_t bgr555  = readShort(f);
        pixel.blue = bgr555 & 0x3F;
        pixel.green = ((bgr555 << 5) & 0x3F) << 1;
        pixel.red = (bgr555 << 10) & 0x3F;
      } else {
        // Pixel data is an index into the palette table
        uint8_t idx;
        switch(bpp) {
        case 8:
          idx = f.read();
          break;
        case 4:
          if(col % 2 == 0) {
            latest = f.read();
            idx = latest >> 4; // most significant nibble has first pixel
          } else {
            idx = latest & 0x0F;
          }
          break;
        case 1:
          if(col % 8 == 0) {
            latest = f.read();
          }
          idx = ( latest & 1<<(7-(col%8)) ) ? 1 : 0; // most significant bit has first pixel
          break;
        }
        pixel = palette[idx];
      }

      assertCS();
      writeData(pixel);
      releaseCS();
    }
  }

  if(bpp != 16)
    free(palette);

  return BMP_OK;
}
