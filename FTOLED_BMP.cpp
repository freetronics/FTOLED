* FTOLED BMP parsing and bitmap display routines for BMP files */

#include <stdint.h>
#include <FTOLED.h>

// Read a little-endian short word from a stream
template<typename T> inline uint16_t readShort(T &s)
{
  return (uint16_t)s.read() | (uint16_t)s.read() << 8;
}

// Read a little-endian long from a stream
template<typename T> inline uint32_t readLong(T &s)
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

// Simple use of _displayBMP uses the SD File object directly
BMP_Status OLED::displayBMP(File &source, const int x, const int y) {
    return _displayBMP(source, x, y);
}

// In order to support Progmem we have a wrapper class that implements the seek()
// and read() methods as inline. This allows the _displayBMP template to compile
// against a PROGMEM stored buffer, using the same source as for a BMP file object!
//
// Yes, this is kinda sorta hacky but it means we don't have separate
// code paths for PROGMEM vs SD stored BMPs. Plus we still have fast code paths for
// both.
#ifdef __AVR__
class _Progmem_wrapper {
  int base;
  int current;
public:
  inline _Progmem_wrapper(const uint8_t *base) : base((int)base),current((int)base) { }
  inline uint8_t read() { return pgm_read_byte(current++); }
  inline void read(void *buf, size_t len) { memcpy_PF(buf, current, len); current += len; }
  inline boolean seek(uint32_t pos) { current=base+pos; return true; }
};
#else
// ARM version is the same, only we just treat the address like a const uint8_t*
//
// Because the ARM isn't Harvard arch this is a little inefficient, in that we're copying
// memory from one part of the address space to another sometimes. But it's not too bad as
// the implementation avoids this where possible to save RAM on the AVRs.
class _Progmem_wrapper {
  const uint8_t *base;
  const uint8_t *current;
public:
  inline _Progmem_wrapper(const uint8_t *base) : base(base),current(base) { }
  inline uint8_t read() { return *current++; }
  inline void read(void *buf, size_t len) { memcpy(buf, current, len); current += len; }
  inline boolean seek(uint32_t pos) { current=base+pos; return true; }
};

#endif

BMP_Status OLED::displayBMP(const uint8_t *pgm_addr, const int x, const int y) {
  _Progmem_wrapper wrapper(pgm_addr);
  return _displayBMP(wrapper, x, y);
}

template<typename SourceType> BMP_Status OLED::_displayBMP(SourceType &source, const int x, const int y)
{
  SourceType &f = source;
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
    uint16_t palette_size = 1<<bpp;
    palette = (Colour *)malloc(sizeof(Colour)*palette_size);
    f.seek(OFFS_DIB_HEADER + dib_headersize);
    for(uint16_t i = 0; i < palette_size; i++) {
      uint8_t pal[3];
      f.read(pal, 3);
      palette[i].blue = pal[0] >> 3;
      palette[i].green = pal[1] >> 2;
      palette[i].red = pal[2] >> 3;
    }
  }

  for(byte row = 0; row < height; row++) {
    f.seek(data_offs + row*row_bytes);
    if(bpp == 16) {
      // TODO: 16bpp support
      /*
      Colour data[out_width];
      uint16_t bgr555  = readShort(f);
      data[col].blue = bgr555 & 0x3F;
      data[col].green = ((bgr555 << 5) & 0x3F) << 1;
      data[col].red = (bgr555 << 10) & 0x3F;
      */
    }
    else if(bpp == 8) {
      uint8_t buf[out_width];
      f.read(&buf, sizeof(buf));
      assertCS();
      for(uint16_t col = 0; col < out_width; col++) {
        writeData(palette[buf[col]]);
      }
      releaseCS();
    }
    else if(bpp == 4) {
      uint8_t buf[(out_width+1)/2];
      f.read(&buf, sizeof(buf));
      assertCS();
      for(uint16_t col = 0; col < out_width/2; col++) {
        writeData(palette[buf[col] >> 4]);
        writeData(palette[buf[col] & 0x0F]);
      }
      if(out_width % 2) { // Odd width, last pixel comes from here
        writeData(palette[buf[sizeof(buf)-1] >> 4]);
      }
      releaseCS();
    }
    else if(bpp == 1) {
      uint8_t buf[(out_width+7)/8];
      f.read(&buf, sizeof(buf));
      assertCS();
      uint8_t bit = 1<<7;
      uint8_t byte = 0;
      for(uint16_t col = 0; col < out_width; col++) {
        writeData(byte & bit ? palette[1] : palette[0]);
        bit >>= 1;
        if(bit == 0) {
          bit = 1<<7;
          byte++;
        }
      }
    }
  }

  if(bpp != 16)
    free(palette);

  return BMP_OK;
}
