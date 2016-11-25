#include "OverworldMarker.h"

OverworldMarker::OverworldMarker(int tilex, int tiley) : OpenGLRectangle(50, 50.0*336.0/512.0, ZeldaInformationHandler::GetTexture(ZeldaInformationHandler::Secrets::UNEXPLORED)) {
  mapx = tilex;
  mapy = tiley;
  secret = ZeldaInformationHandler::Secrets::UNKNOWN_CAVE;
}

void OverworldMarker::Update() {
  ZeldaInformationHandler::Secrets newSecret = ZeldaInformationHandler::GetSecret(mapx, mapy);
  if (newSecret != secret) {
    secret = newSecret;
    SetTexture(ZeldaInformationHandler::GetTexture(secret));
  }
}

void OverworldMarker::Draw(double time) {
  if (secret != ZeldaInformationHandler::Secrets::UNEXPLORED) {
    OpenGLRectangle::Draw(time);
  }
}
