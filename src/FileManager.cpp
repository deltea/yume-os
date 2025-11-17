#include "FileManager.h"
#include "utils.h"

void FileManager::indexSongs(const String& path, vector<String>& tracks) {
  File root = SD.open(path);
  if (!root || !root.isDirectory()) {
    Serial.println("failed to open directory");
    return;
  }

  File file = root.openNextFile();
  while (file) {
    if (!file.isDirectory()) break;

    Serial.println("found directory: " + String(file.name()));
    // load first playlist file found
    if (String(file.name()) == "playlists") {
      File playlistFile = SD.open(file.path()).openNextFile();
      if (playlistFile) {
        Serial.println("loading playlist \"" + String(playlistFile.name()) + "\"...");
        JsonDocument doc;
        DeserializationError error = deserializeJson(doc, playlistFile);
        if (!error) {
          Serial.println("playlist tracks: " + String(doc["tracks"].as<String>()));
          JsonArray jsonTracks = doc["tracks"].as<JsonArray>();
          tracks.clear();
          for (JsonVariant track : jsonTracks) {
            tracks.push_back(track.as<String>());
          }
        }

        playlistFile.close();
      }
    }

    file = root.openNextFile();
  }
}



Track FileManager::getTrack(const String& path) {
  File jsonFile = SD.open("/tracks/" + path + "/track.json");
  if (!jsonFile) {
    Serial.println("failed to open track file: " + path);
    return Track();
  }

  JsonDocument doc;
  DeserializationError error = deserializeJson(doc, jsonFile);
  if (error) {
    Serial.println("failed to read track metadata for " + String(path) + ": " + String(error.c_str()));
    jsonFile.close();
    return Track();
  }

  Track track;
  track.title = doc["title"].as<String>();
  track.artist = doc["artist"].as<String>();
  track.duration = doc["duration"].as<uint16_t>();
  track.color = doc["color"].as<uint16_t>();
  track.cover_path = "/tracks/" + String(path) + "/cover.raw";
  track.audio_path = "/tracks/" + String(path) + "/audio.mp3";
  Serial.println("retrieved track: " + track.title + " by " + track.artist);

  jsonFile.close();
  return track;
}
