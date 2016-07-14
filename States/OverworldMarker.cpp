#include "OverworldMarker.h"
#include "ZeldaInformationHandler.h"

OverworldMarker::OverworldMarker(int tilex, int tiley) : OpenGLRectangle(512, 336, "Images/Selectors/OverworldExplored.png") {
  mapx = tilex;
  mapy = tiley;
  secret = false;
}

void OverworldMarker::Update() {
  if (!secret) {
    if (ZeldaInformationHandler::GetSecret(mapx, mapy) == ZeldaInformationHandler::Secrets::UNKNOWN_CAVE
	|| ZeldaInformationHandler::GetSecret(mapx, mapy) == ZeldaInformationHandler::Secrets::UNKNOWN_DUNGEON) {
      secret = true;
    }
  }
}

void OverworldMarker::Draw(double time) {
  if (secret) {
    OpenGLRectangle::Draw(time);
  }
}
