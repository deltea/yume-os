#pragma once

#include <vector>
#include <Adafruit_GFX.h>

#include "Screen.h"
#include "AppContext.h"

using namespace std;

class LibraryScreen : public Screen {
  private:
    const vector<String> OPTIONS = {"playlists", "artists", "albums", "songs"};

    int select_index;

  public:
    LibraryScreen(AppContext* ctx) : Screen(ctx) {};

    void init() override;
    void update() override;
    void draw() override;
};
