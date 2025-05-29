#pragma once

using namespace std;

#include <vector>
#include <Adafruit_GFX.h>

struct Track {
  String title;
  String artist;
  uint16_t duration;
  uint16_t color;
};

class AppState {
  public:
    vector<Track> queue = {
      {"last surprise", "persona 5 ost", 268, 0xFF00},
      {"blooming villager", "animal crossing new horizons", 120, 0xF000},
      {"sakura", "persona 5 royal", 180, 0x00FF}
    };

    uint16_t currentIndex = 0;
    bool isPlaying = false;
    int batteryLevel = 100;

    Track getCurrentTrack();
    void setBatteryLevel(int level);
};
