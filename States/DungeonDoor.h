#ifndef DungeonDoor_1468690097206650
#define DungeonDoor_1468690097206650

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <memory>
#include <cassert>
#include <limits>
#include <chrono>
#include <set>
#include <cstdlib>
#include <tuple>
#include <map>
#include <thread>
#include <mutex>

#include "OpenGLRectangle.h"
#include "ZeldaInformationHandler.h"

class DungeonDoor : public OpenGLRectangle {
 public:
  DungeonDoor(int x1, int y1, int x2, int y2);
  void Update() override;
  void Draw(double time) override;
 private:
  std::tuple<int, int, int, int> doorLoc;
  Dungeon::DoorType type;
};

#endif
