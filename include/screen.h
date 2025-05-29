#pragma once

#include <Adafruit_GFX.h>

class Screen {
  public:
    virtual ~Screen() {};

    virtual void init() = 0;
    virtual void update() = 0;
    virtual void draw() = 0;
};
