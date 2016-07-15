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
  enum class Secrets {UNEXPLORED, UNKNOWN_CAVE, UNKNOWN_DUNGEON, DUNGEON_1, DUNGEON_2,
      DUNGEON_3, DUNGEON_4, DUNGEON_5, DUNGEON_6, DUNGEON_7, DUNGEON_8, DUNGEON_9};
  enum class RoomType {UNEXPLORED, UNKNOWN_ROOM};    
  
  static void SetMapLocation(int x, int y);
  static std::pair<int, int> GetMapLocation();
  static void SetDungeonLocation(int x, int y, RoomType type);
  static std::pair<int, int> GetDungeonLocation();
  static RoomType GetDungeonRoomType(int x, int y);
  static void SetIsRunning(bool running);
  static bool GetIsRunning();
  static bool GetIsInDungeon();

  static Secrets GetSecret(int x, int y);
  static void SetSecret(int x, int y, Secrets secret);
  
 private:
  static std::recursive_mutex dataMutex;

  static int mapx;
  static int mapy;

  static int dungeonx;
  static int dungeony;
  class Dungeon {
  public:
    Dungeon(int overworldx, int overworldy);
    void SetLocation(int x, int y, RoomType type);
    RoomType GetRoomType(int x, int y);
    std::pair<int, int> GetLocation();
    Secrets Number();
  private:
    int overworldx;
    int overworldy;
    Secrets levelNumber;
    std::map<std::pair<int, int>, RoomType> rooms;
  };
  static std::vector<Dungeon> dungeons;
  
  static void SetIsInDungeon(bool isInDungeon);
  static bool isInDungeon;

  static std::map<std::pair<int,int>, Secrets> overworldSecrets;

  static bool isRunning;
};

#endif
