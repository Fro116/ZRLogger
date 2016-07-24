#include "ItemMarker.h"

ItemMarker::ItemMarker(int levelNumber, bool firstItem) : OpenGLRectangle(32, 32, "Images/Selectors/PreItem.png") {
  level = levelNumber;
  first = firstItem;
  item = ZeldaInformationHandler::DungeonItems::NONE;
}

void ItemMarker::Update() {
  ZeldaInformationHandler::DungeonItems newitem = ZeldaInformationHandler::GetItem(level, first);
  if (newitem != item) {
    item = newitem;
    SetTexture(ZeldaInformationHandler::GetTexture(item));
  }
}

void ItemMarker::Draw(double time) {
  OpenGLRectangle::Draw(time);
}

