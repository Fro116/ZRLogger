#ifndef Dungeon_1479925239067147
#define Dungeon_1479925239067147

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

class Dungeon {
 public:
  Dungeon(int overworldx, int overworldy);

  enum class DungeonType {UNKNOWN_DUNGEON, DUNGEON_1, DUNGEON_2, DUNGEON_3, DUNGEON_4, DUNGEON_5, DUNGEON_6, DUNGEON_7, DUNGEON_8, DUNGEON_9};   
  enum class RoomType {UNEXPLORED, UNKNOWN_ROOM, UNSEEN_ROOM, GUESS_ROOM, TRIFORCE_ROOM,
      STAIRCASE_1, STAIRCASE_2, STAIRCASE_3, STAIRCASE_4, STAIRCASE_5, STAIRCASE_6, STAIRCASE_7};
  enum class DoorType {UNEXPLORED, OPEN, KEY, SHUTTER, BOMB}; 
  enum class DungeonItems {BOOK, BOW, HEART_CONTAINER, LADDER, MAGICAL_BOOMERANG, MAGICAL_KEY, NONE, RAFT, RECORDER, RED_CANDLE,
      RED_RING, POWER_BRACELET, SILVER_ARROW, WAND, WHITE_SWORD, WOODEN_BOOMERANG};    

  void SetLevelNine();
  void SetLocation(int x, int y, RoomType type);
  void SetDoorType(int x1, int y1, int x2, int y2, DoorType type);
  DoorType GetDoorType(int x1, int y1, int x2, int y2);    
  RoomType GetRoomType(int x, int y);
  std::pair<int, int> GetLocation();
  std::pair<int, int> GetStartLocation();    
  DungeonType Number();
  void SetTriforce();
  bool GetTriforce();
  void SetHeart();
  bool GetHeart();
  void SetItem(DungeonItems item, std::pair<int, int> loc);
  DungeonItems GetFirstItem();
  DungeonItems GetSecondItem();
  DungeonItems GetItem(std::pair<int, int> loc);
  void SetStaircase(std::pair<int, int> firstroom, std::pair<int, int> secondroom);    
 private:
  int overworldx;
  int overworldy;
  DungeonType levelNumber;
  bool triforce;
  bool heart;
  std::pair<int, int> startloc;    
  DungeonItems firstItem;
  std::pair<int, int> firstitemloc;
  DungeonItems secondItem;
  std::pair<int, int> seconditemloc;    
  std::map<std::pair<int, int>, RoomType> rooms;
  std::map<std::tuple<int, int, int, int>, DoorType> doors;
  int staircaseNumber;
};

#endif
