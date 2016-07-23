#include "HeartMarker.h"

HeartMarker::HeartMarker(int levelNumber) : OpenGLRectangle(32, 32, "Images/Selectors/PreHeart.png") {
  level = levelNumber;
  hasHeart = false;
}

void HeartMarker::Update() {
  bool heart = ZeldaInformationHandler::GetHeart(level);
  if (heart != hasHeart) {
    hasHeart = heart;
    SetTexture("Images/Selectors/PostHeart.png");
  }
}

void HeartMarker::Draw(double time) {
  OpenGLRectangle::Draw(time);
}
