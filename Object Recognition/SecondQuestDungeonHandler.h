#ifndef SecondQuestDungeonHandler_1480005164188665
#define SecondQuestDungeonHandler_1480005164188665

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

class SecondQuestDungeonHandler : public DungeonHandler {
 public:
  SecondQuestDungeonHandler();
  void PredictLevel(Dungeon& dungeon);
 private:
  std::vector<std::vector<std::vector<bool>>> dungeonShapes;
};

#endif
