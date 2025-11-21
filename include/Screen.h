#pragma once

#include "Context.h"

class Screen {
  protected:
    Context* ctx;

  public:
    Screen(Context* ctx) : ctx(ctx) {}
    virtual ~Screen() = default;

    virtual void init() = 0;
    virtual void update() = 0;
    virtual void draw() = 0;
};
