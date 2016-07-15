#include "DungeonMarker.h"

DungeonMarker::DungeonMarker(int tilex, int tiley) : OpenGLRectangle(100, 100.0*336.0/1024.0, ZeldaInformationHandler::GetTexture(ZeldaInformationHandler::RoomType::UNEXPLORED)) {
  mapx = tilex;
  mapy = tiley;
  type = ZeldaInformationHandler::RoomType::UNEXPLORED;
}

void DungeonMarker::Update() {
  ZeldaInformationHandler::RoomType newType = ZeldaInformationHandler::GetDungeonRoomType(mapx, mapy);
  if (newType != type) {
    type = newType;
    SetTexture(ZeldaInformationHandler::GetTexture(type));
  }
}

void DungeonMarker::Draw(double time) {
  if (type != ZeldaInformationHandler::RoomType::UNEXPLORED) {
    OpenGLRectangle::Draw(time);
  }
}
