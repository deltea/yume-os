#pragma once

using namespace std;

#include <vector>
#include <Adafruit_GFX.h>
#include "Track.h"

class State {
  public:
    vector<String> queue = {};
    vector<Track> library = {};

    uint16_t currentIndex = 0;
    bool isPlaying = false;
    int batteryLevel = 100;

    String getCurrentTrackName();
    void setBatteryLevel(int level);
};
