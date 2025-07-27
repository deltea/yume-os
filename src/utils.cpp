#include "utils.h"

// int getTextWidth(const GFXfont* font, const String& text) {
//   int width = 0;
//   for (char c : text) {
//     width += pgm_read_byte(&font->glyph[c - font->first].width);
//   }

//   return width;
// }

int getTextWidth(const String& text) {
  return text.length() * 6;
}

uint16_t hexToRGB565(const char* hex) {
  uint8_t r, g, b;
  sscanf(hex + 1, "%02hhx%02hhx%02hhx", &r, &g, &b);
  return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}
