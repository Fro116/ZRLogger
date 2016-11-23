#ifndef DungeonMarker_1468597539936446
#define DungeonMarker_1468597539936446

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

class DungeonMarker : public OpenGLRectangle {
 public:
  DungeonMarker(int tilex, int tiley);
  void Update() override;
  void Draw(double time) override;
 private:
  int mapx;
  int mapy;
  Dungeon::RoomType type;
};

#endif
