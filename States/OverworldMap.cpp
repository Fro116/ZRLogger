#include "OverworldMap.h"

OverworldMap::OverworldMap() : OpenGLRectangle(800, 262.5, "Images/Selectors/FirstQuestOverworld.png"), questKnown(false) {
  
}

void OverworldMap::Update() {
  if (!questKnown && ZeldaInformationHandler::GetIsOptionsInitialized()) {
    questKnown = true;
    if (!ZeldaInformationHandler::GetQuest())
      SetTexture("Images/Selectors/SecondQuestOverworld.png");
  }
}
