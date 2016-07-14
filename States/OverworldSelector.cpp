#include "OverworldSelector.h"

OverworldSelector::OverworldSelector() : OpenGLRectangle(512, 336, "Images/Selectors/OverworldSelector.png") {
  mapx = 0;
  mapy = 0;
}

void OverworldSelector::Update() {
  double px = 50;
  double py = 50.0*336.0/512.0;
  MoveTo(glm::vec3(px*mapx + px/2, py*(7-mapy)+py/2,0.001));
}
