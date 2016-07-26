#ifndef BonusCaveMarker_1469554585070792
#define BonusCaveMarker_1469554585070792

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

class BonusCaveMarker : public OpenGLRectangle {
 public:
  BonusCaveMarker(int number);
  void Update() override;
  void Draw(double time) override;
 private:
  int number;
  bool hasBonusCave;
};

#endif
