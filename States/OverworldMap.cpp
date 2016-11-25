#include "OverworldMap.h"

OverworldMap::OverworldMap() : OpenGLRectangle(800, 262.5, "Images/Selectors/SecondQuestOverworld.png"), questKnown(false) {
  
}

void OverworldMap::Update() {
  // bool newQuest = ZeldaInformationHandler::GetQuest();
  // if (newQuest != firstQuest) {
  //   firstQuest = newQuest;
  //   SetTexture("Images/Selectors/SecondQuestOverworld.png");
  // }
}
