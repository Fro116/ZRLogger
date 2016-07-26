#include "BonusCaveMarker.h"

BonusCaveMarker::BonusCaveMarker(int levelNumber) : OpenGLRectangle(32, 32, "Images/Selectors/PreBonusCave.png") {
  number = levelNumber;
  hasBonusCave = false;
}

void BonusCaveMarker::Update() {
  int numcaves = ZeldaInformationHandler::GetBonusCaves();
  if (numcaves > number) {
    hasBonusCave = true;
    SetTexture("Images/Selectors/PostBonusCave.png");
  }
}

void BonusCaveMarker::Draw(double time) {
  OpenGLRectangle::Draw(time);
}

