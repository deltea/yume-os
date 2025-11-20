#pragma once

class State;
class AudioManager;
class FileManager;
class InputManager;
class ScreenManager;
class GFXcanvas16;

struct AppContext {
  State* state;
  AudioManager* audio;
  FileManager* files;
  InputManager* input;
  ScreenManager* screens;
  GFXcanvas16* canvas;
};
