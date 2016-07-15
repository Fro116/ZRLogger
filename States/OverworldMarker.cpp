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
  }
}

void OverworldMarker::Draw(double time) {
  if (secret != ZeldaInformationHandler::Secrets::UNEXPLORED) {
    OpenGLRectangle::Draw(time);
  }
}
