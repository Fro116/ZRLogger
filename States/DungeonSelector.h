#ifndef DungeonSelector_1468603407321801
#define DungeonSelector_1468603407321801

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

class DungeonSelector : public OpenGLRectangle {
 public:
  DungeonSelector();
  void Update() override;
 private:
  int mapx;
  int mapy;
};

#endif
