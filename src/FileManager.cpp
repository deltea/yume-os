#include "FileManager.h"
#include "utils.h"

void FileManager::indexSongs(const String& path, vector<Track>& tracks) {
  File root = SD.open(path);
  if (!root || !root.isDirectory()) {
    Serial.println("failed to open directory");
    return;
  }

  File file = root.openNextFile();
  while (file) {
    if (file.isDirectory()) {
      File song = SD.open(file.path());
      if (song) {
        File jsonFile = SD.open(String(song.path()) + "/track.json");
        File coverFile = SD.open(String(song.path()) + "/cover.raw");

        // add track metadata to library
        JsonDocument doc;
        DeserializationError error = deserializeJson(doc, jsonFile);
        if (!error) {
          Track track;
          track.title = doc["title"].as<String>();
          track.artist = doc["artist"].as<String>();
          track.duration = doc["duration"].as<uint16_t>();
          track.color = hexToRGB565(doc["accent_color"].as<const char*>());

          // add cover image to library
          if (coverFile) {
            track.cover_file = coverFile;
            // coverFile.read((uint8_t*)track.cover, sizeof(track.cover));
            // coverFile.close();
          }

          tracks.push_back(track);
          Serial.println("added track: " + track.title + " by " + track.artist);
        }

        jsonFile.close();
      }
    }

    file = root.openNextFile();
  }
}
