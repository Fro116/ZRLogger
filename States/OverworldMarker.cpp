#include "OverworldMarker.h"

OverworldMarker::OverworldMarker(int tilex, int tiley) : OpenGLRectangle(50, 50.0*336.0/512.0, "Images/Selectors/OverworldExplored.png") {
  mapx = tilex;
  mapy = tiley;
  secret = ZeldaInformationHandler::Secrets::UNEXPLORED;
}

void OverworldMarker::Update() {
  ZeldaInformationHandler::Secrets newSecret = ZeldaInformationHandler::GetSecret(mapx, mapy);
  if (newSecret != secret) {
    secret = newSecret;
    if (secret == ZeldaInformationHandler::Secrets::UNKNOWN_CAVE) {
      SetTexture("Images/Selectors/OverworldExplored.png");
    }
    if (secret == ZeldaInformationHandler::Secrets::UNKNOWN_DUNGEON) {
      SetTexture("Images/Selectors/OverworldUnkownDungeon.png");
    }
    if (secret == ZeldaInformationHandler::Secrets::DUNGEON_1) {
      SetTexture("Images/Selectors/OverworldDungeon1.png");
    }
    if (secret == ZeldaInformationHandler::Secrets::DUNGEON_2) {
      SetTexture("Images/Selectors/OverworldDungeon2.png");
    }
    if (secret == ZeldaInformationHandler::Secrets::DUNGEON_3) {
      SetTexture("Images/Selectors/OverworldDungeon3.png");
    }
    if (secret == ZeldaInformationHandler::Secrets::DUNGEON_4) {
      SetTexture("Images/Selectors/OverworldDungeon4.png");
    }
    if (secret == ZeldaInformationHandler::Secrets::DUNGEON_5) {
      SetTexture("Images/Selectors/OverworldDungeon5.png");
    }
    if (secret == ZeldaInformationHandler::Secrets::DUNGEON_6) {
      SetTexture("Images/Selectors/OverworldDungeon6.png");
    }
    if (secret == ZeldaInformationHandler::Secrets::DUNGEON_7) {
      SetTexture("Images/Selectors/OverworldDungeon7.png");
    }
    if (secret == ZeldaInformationHandler::Secrets::DUNGEON_8) {
      SetTexture("Images/Selectors/OverworldDungeon8.png");
    }
    if (secret == ZeldaInformationHandler::Secrets::DUNGEON_9) {
      SetTexture("Images/Selectors/OverworldDungeon9.png");
    }
  }
}

void OverworldMarker::Draw(double time) {
  if (secret != ZeldaInformationHandler::Secrets::UNEXPLORED) {
    OpenGLRectangle::Draw(time);
  }
}
