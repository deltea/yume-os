#pragma once

#include "screen.h"

class Screen {
  public:
    virtual void init() = 0;
    virtual void update() = 0;
    virtual void draw() = 0;
};
