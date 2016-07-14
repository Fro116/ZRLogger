#ifndef OverworldSelector_1468460211736556
#define OverworldSelector_1468460211736556

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

#include "OpenGLRectangle.h"

class OverworldSelector : public OpenGLRectangle {
 public:
  OverworldSelector();
  void Update() override;
 private:
  int mapx;
  int mapy;
};

#endif
