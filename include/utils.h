#pragma once

using namespace std;

#include <Adafruit_GFX.h>

// int getTextWidth(const GFXfont* font, const String& text);
int getTextWidth(const String& text);
uint16_t hexToRGB565(const char* hex);
String getTimestamp(int value);
