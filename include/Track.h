#pragma once

#include <SD.h>

struct Track {
  String title;
  String artist;
  uint16_t duration;
  uint16_t color;
  String cover_path;
  String audio_path;
};
