#include "OverworldMarker.h"
#include "ZeldaInformationHandler.h"

OverworldMarker::OverworldMarker(int tilex, int tiley) : OpenGLRectangle(512, 336, "Images/Selectors/OverworldExplored.png") {
  mapx = tilex;
  mapy = tiley;
  secret = false;
}

void OverworldMarker::Update() {
  if (!secret) {
    std::tuple<int, int, bool> coor = ZeldaInformationHandler::GetMapLocation();
    int cx = std::get<0>(coor);
    int cy = std::get<1>(coor);
    bool cs = std::get<2>(coor);
    if (cx == mapx && cy == mapy && cs) {
      secret = true;
    }
  }
}

void OverworldMarker::Draw(double time) {
  if (secret) {
    OpenGLRectangle::Draw(time);
  }
}
