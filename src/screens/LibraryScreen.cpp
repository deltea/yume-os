#include <SD.h>
#include "screens/LibraryScreen.h"
#include "constants.h"
#include "cutepixel.h"
#include "monogram.h"

LibraryScreen::LibraryScreen(ScreenManager* screenManager, GFXcanvas16* canvas, State* state, InputManager* inputManager)
  : screenManager(screenManager), canvas(canvas), state(state), inputManager(inputManager) {};

void LibraryScreen::init() {
  canvas->setTextColor(FG);
  canvas->setTextWrap(false);
  canvas->setTextSize(1);
}

void LibraryScreen::update() {

}

void LibraryScreen::draw() {

}
