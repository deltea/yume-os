#pragma once

using namespace std;

#include <list>
#include "Screen.h"

class ScreenManager {
  private:
    Screen* currentScreen = nullptr;

  public:
    void setScreen(Screen* screen);
    void init();
    void update();
    void draw();
};
