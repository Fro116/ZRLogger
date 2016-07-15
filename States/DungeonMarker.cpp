#include "DungeonMarker.h"

DungeonMarker::DungeonMarker(int tilex, int tiley) : OpenGLRectangle(100, 100.0*336.0/1024.0, "Images/Selectors/DungeonRoom.png") {
  mapx = tilex;
  mapy = tiley;
  type = ZeldaInformationHandler::RoomType::UNEXPLORED;
}

void DungeonMarker::Update() {
  ZeldaInformationHandler::RoomType newType = ZeldaInformationHandler::GetDungeonRoomType(mapx, mapy);
  if (newType != type) {
    type = newType;
    if (type == ZeldaInformationHandler::RoomType::UNKNOWN_ROOM) {
      SetTexture("Images/Selectors/DungeonRoom.png");
    }
  }
}

void DungeonMarker::Draw(double time) {
  if (type != ZeldaInformationHandler::RoomType::UNEXPLORED) {
    OpenGLRectangle::Draw(time);
  }
}
