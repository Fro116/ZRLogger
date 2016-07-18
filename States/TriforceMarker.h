#ifndef TriforceMarker_1468879529250482
#define TriforceMarker_1468879529250482

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

class TriforceMarker : public OpenGLRectangle {
 public:
  TriforceMarker(int level);
  void Update() override;
  void Draw(double time) override;
 private:
  int level;
  bool hasTriforce;
};

#endif
