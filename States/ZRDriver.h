#ifndef ZRDriver_1468473517326545
#define ZRDriver_1468473517326545

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

#include "GameDriver.h"

class ZRDriver : public GameDriver {
 public:
  ZRDriver(std::string title, int width, int height, long nanosPerFrame, int maxSkippedDrawSteps);
  void Run() override;
 private:
};

#endif
