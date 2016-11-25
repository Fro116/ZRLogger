#ifndef DungeonHandler_1480002471274610
#define DungeonHandler_1480002471274610

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

#include "Dungeon.h"

class DungeonHandler {
 public:
  DungeonHandler();
  virtual void PredictLevel(Dungeon& dungeon) = 0;
 protected:
  std::vector<std::vector<bool>> FormatShape(int shape[]);
  Dungeon::DungeonType AsDungeonType(int level);
  int AsNumber(Dungeon::DungeonType level);
 private:
};

#endif
