#include <SD.h>
#include "screens/MovieScreen.h"
#include "constants.h"
#include "cutepixel.h"
#include "monogram.h"

MovieScreen::MovieScreen(ScreenManager* screenManager, GFXcanvas16* canvas, State* state)
  : screenManager(screenManager), canvas(canvas), state(state) {};

void MovieScreen::init() {
  canvas->setTextColor(FG);
  canvas->setTextWrap(false);
  canvas->setTextSize(1);
}

void MovieScreen::update() {

}

void MovieScreen::draw() {
  
}
