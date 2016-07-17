#ifndef ZeldaFindingScreen_1468720249436993
#define ZeldaFindingScreen_1468720249436993

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

class ZeldaFindingScreen : public OpenGLRectangle {
 public:
  ZeldaFindingScreen();
  void Update() override;
  void Draw(double time) override;
 private:
  bool found;
};

#endif
