#ifndef HeartMarker_1469304807003377
#define HeartMarker_1469304807003377

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

class HeartMarker : public OpenGLRectangle {
 public:
  HeartMarker(int level);
  void Update() override;
  void Draw(double time) override;  
 private:
  int level;
  bool hasHeart;  
};

#endif
