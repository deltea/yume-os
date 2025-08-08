#pragma once

#include <SD.h>
#include <vector>
#include <ArduinoJson.h>
#include "Track.h"

using namespace std;

class FileManager {
  public:
    void indexSongs(const String& path, vector<String>& tracks);
    Track getTrack(const String& path);
};
