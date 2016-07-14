#ifndef ZeldaInformationHandler_1468462295480346
#define ZeldaInformationHandler_1468462295480346

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

class ZeldaInformationHandler {
 public:
  ZeldaInformationHandler();
  void SetMapLocation(int x, int y);
  std::pair<int, int> GetMapLocation();
 private:
  std::mutex mapLocationMutex;
  int mapx;
  int mapy;
};

#endif
