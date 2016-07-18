#include "TriforceMarker.h"

TriforceMarker::TriforceMarker(int levelNumber) : OpenGLRectangle(40, 40, "Images/Selectors/PreTriforce.png") {
  level = levelNumber;
  hasTriforce = false;
}

void TriforceMarker::Update() {
  bool triforce = ZeldaInformationHandler::GetTriforce(level);
  if (triforce != hasTriforce) {
    hasTriforce = triforce;
    SetTexture("Images/Selectors/PostTriforce.png");
  }
}

void TriforceMarker::Draw(double time) {
  OpenGLRectangle::Draw(time);
}

