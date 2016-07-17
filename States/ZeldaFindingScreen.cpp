#include "ZeldaFindingScreen.h"

ZeldaFindingScreen::ZeldaFindingScreen() : OpenGLRectangle(800, 400, "Images/ZeldaFindingScreen.png") {
  found = false;
}

void ZeldaFindingScreen::Update() {
  if (!found) {
    found = ZeldaInformationHandler::GetZeldaSceenFound();
  }
}

void ZeldaFindingScreen::Draw(double time) {
  if (!found) {
    OpenGLRectangle::Draw(time);
  }
}
