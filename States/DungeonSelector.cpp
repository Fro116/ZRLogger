#include "DungeonSelector.h"
#include "ZeldaInformationHandler.h"

DungeonSelector::DungeonSelector() : OpenGLRectangle(100, 100.0*336.0/1024.0, "Images/Selectors/DungeonSelector.png") {
  mapx = 0;
  mapy = 0;
}

void DungeonSelector::Update() {
  std::pair<int, int> coor = ZeldaInformationHandler::GetDungeonLocation();
  mapx = coor.first;
  mapy = coor.second;
  double px = 100;
  double py = px*336.0/1024.0;
  MoveTo(glm::vec3(px*mapx + px/2, py*(7-mapy)+py/2,0.002));
}
