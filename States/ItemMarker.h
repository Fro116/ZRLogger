#ifndef ItemMarker_1469316227691105
#define ItemMarker_1469316227691105

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

class ItemMarker : public OpenGLRectangle {
 public:
  ItemMarker(int level, bool firstItem);
  void Update() override;
  void Draw(double time) override;
 private:
  int level;
  bool first;
  ZeldaInformationHandler::DungeonItems item;
};

#endif
