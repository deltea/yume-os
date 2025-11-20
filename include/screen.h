#pragma once

#include "AppContext.h"

class Screen {
  protected:
    AppContext* ctx;

  public:
    Screen(AppContext* ctx) : ctx(ctx) {}
    virtual ~Screen() = default;

    virtual void init() = 0;
    virtual void update() = 0;
    virtual void draw() = 0;
};
