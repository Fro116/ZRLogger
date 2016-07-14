#ifndef OverworldMarker_1468519670471386
#define OverworldMarker_1468519670471386

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

class OverworldMarker : public OpenGLRectangle {
 public:
  OverworldMarker(int tilex, int tiley);
  void Update() override;
  void Draw(double time) override;
 private:
  int mapx;
  int mapy;
  bool secret;
};

#endif
