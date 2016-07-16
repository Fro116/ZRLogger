#include "DungeonDoor.h"

DungeonDoor::DungeonDoor(int x1, int y1, int x2, int y2) : OpenGLRectangle(30, 15, ZeldaInformationHandler::GetTexture(ZeldaInformationHandler::RoomType::UNKNOWN_ROOM)) {
  doorLoc = std::make_tuple(x1, y1, x2, y2);
  type = ZeldaInformationHandler::DoorType::UNEXPLORED;
}

void DungeonDoor::Update() {
  ZeldaInformationHandler::DoorType newType = ZeldaInformationHandler::GetDungeonDoor(std::get<0>(doorLoc), std::get<1>(doorLoc), std::get<2>(doorLoc), std::get<3>(doorLoc));
  if (newType != type) {
    type = newType;
    // SetTexture(ZeldaInformationHandler::GetTexture(type));
  }
}

void DungeonDoor::Draw(double time) {
  if (type != ZeldaInformationHandler::DoorType::UNEXPLORED) {
    OpenGLRectangle::Draw(time);
  }
}
