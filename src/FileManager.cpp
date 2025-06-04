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
    if (!file.isDirectory()) break;

    File json_file = SD.open(String(file.path()) + "/track.json");

    if (!json_file) {
      Serial.println("failed to open track.json in " + String(file.path()));
      file = root.openNextFile();
      continue;
    }

    // add track metadata to library
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, json_file);
    if (!error) {
      Track track;
      track.title = doc["title"].as<String>();
      track.artist = doc["artist"].as<String>();
      track.duration = doc["duration"].as<uint16_t>();
      track.color = doc["color"].as<uint16_t>();

      // add cover image path to library
      track.cover_path = String(file.path()) + "/cover.raw";

      tracks.push_back(track);
      Serial.println("added track: " + track.title + " by " + track.artist);
    } else {
      Serial.println("failed to read track metadata for " + String(file.path()));
    }

    json_file.close();
    file = root.openNextFile();
  }
}
