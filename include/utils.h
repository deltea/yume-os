#pragma once

using namespace std;

#include <Adafruit_GFX.h>

int getTextWidth(const GFXfont* font, const String& text) {
  int width = 0;
  for (char c : text) {
    width += pgm_read_byte(&font->glyph[c - font->first].width);
  }

  return width;
}
