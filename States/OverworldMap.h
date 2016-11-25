#ifndef OverworldMap_1479962059283274
#define OverworldMap_1479962059283274

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
#include <unordered_set>
#include <cstdlib>
#include <tuple>
#include <map>
#include <unordered_map>
#include <thread>
#include <mutex>

#include "OpenGLRectangle.h"
#include "ZeldaInformationHandler.h"

class OverworldMap : public OpenGLRectangle {
 public:
  OverworldMap();
  void Update() override;
 private:
  bool questKnown;
};

#endif
