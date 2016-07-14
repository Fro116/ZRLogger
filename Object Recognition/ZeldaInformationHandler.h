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
#include <mutex>

class ZeldaInformationHandler {
 public:
  ZeldaInformationHandler();
  static void SetMapLocation(int x, int y);
  static std::pair<int, int> GetMapLocation();
  static void SetIsRunning(bool running);
  static bool GetIsRunning();
 private:
  static std::mutex mapLocationMutex;
  static int mapx;
  static int mapy;

  static std::mutex isRunningMutex;
  static bool isRunning;
};

#endif
