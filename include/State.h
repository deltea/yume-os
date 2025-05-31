#pragma once

using namespace std;

#include <vector>
#include <Adafruit_GFX.h>
#include "Track.h"

class State {
  public:
    vector<Track> queue = {};
    vector<Track> library = {};

    uint16_t currentIndex = 2;
    bool isPlaying = false;
    int batteryLevel = 100;

    Track getCurrentTrack();
    void setBatteryLevel(int level);
};
