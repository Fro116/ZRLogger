#ifndef RandomDungeonShapeHandler_1480003424622155
#define RandomDungeonShapeHandler_1480003424622155

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

#include "DungeonHandler.h"

class RandomDungeonShapeHandler : public DungeonHandler {
 public:
  RandomDungeonShapeHandler();
  void PredictLevel(Dungeon& dungeon);
 private:
};

#endif
