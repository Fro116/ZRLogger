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

#include "OpenGLUtility.h"
#include "Dungeon.h"

class ZeldaInformationHandler {
 public:
  enum class Secrets {UNEXPLORED, UNKNOWN_CAVE, UNKNOWN_DUNGEON, DUNGEON_1, DUNGEON_2,
      DUNGEON_3, DUNGEON_4, DUNGEON_5, DUNGEON_6, DUNGEON_7, DUNGEON_8, DUNGEON_9,
      CANDLE_SHOP, BAIT_SHOP, BOMB_SHOP, ARROW_SHOP, BLUE_RING_SHOP, WOOD_SWORD, WHITE_SWORD,
      MAGICAL_SWORD, PRE_POTION_SHOP, POTION_SHOP, EXPLORED_CAVE, BONUS_CAVE, ANYROAD};

  static void Init();
  static void InitTextures();

  static void SetZeldaSceenFound(bool found);
  static bool GetZeldaSceenFound();  

  static void SetIsRunning(bool running);
  static bool GetIsRunning();

  static void SetMapLocation(int x, int y);
  static std::pair<int, int> GetMapLocation();
  static void SetDungeonLocation(int x, int y, Dungeon::RoomType type);
  static std::pair<int, int> GetDungeonLocation();  

  static Secrets GetSecret(int x, int y);
  static void SetSecret(int x, int y, Secrets secret);
  static int GetHearts();
  static int GetBonusCaves();    
  
  static Dungeon::RoomType GetDungeonRoomType(int x, int y);
  static Dungeon::DoorType GetDungeonDoor(int x1, int y1, int x2, int y2);
  static void SetDungeonDoor(int x1, int y1, int x2, int y2, Dungeon::DoorType type);
  static void SetStaircase();    
  static bool GetIsInDungeon();

  static bool GetTriforce(int level); //zero indexed
  static void SetTriforce();
  static void SetHearts(int numHearts);
  static bool GetHeart(int level); //zero indexed
  static void SetItem(Dungeon::DungeonItems item);
  static Dungeon::DungeonItems GetItem(int level, bool first);  

  static GLuint GetTexture(Secrets type);
  static GLuint GetTexture(Dungeon::RoomType type);
  static GLuint GetTexture(Dungeon::DoorType type);
  static GLuint GetTexture(Dungeon::DungeonItems type);  
 private:
  static Secrets AsSecret(Dungeon::DungeonType type);
  static Secrets AsSecret(int dungeonLevel);  
  static std::recursive_mutex dataMutex;

  static bool zeldaScreenFound;

  static std::map<Secrets, GLuint> overworldTextures;
  static std::map<Dungeon::RoomType, GLuint> dungeonTextures;
  static std::map<Dungeon::DoorType, GLuint> doorTextures;
  static std::map<Dungeon::DungeonItems, GLuint> itemTextures;    
  static std::map<std::pair<int,int>, Secrets> overworldSecrets;
  
  static int mapx;
  static int mapy;
  static int dungeonx;
  static int dungeony;

  static std::vector<Dungeon> dungeons;
  static std::vector<std::vector<std::vector<bool>>> dungeonShapes;
  static std::vector<std::vector<bool>> FormatShape(int shape[]);
  
  static void SetIsInDungeon(bool isInDungeon);
  static bool isInDungeon;
  static bool isStaircase;
  
  static int hearts;
  
  static bool isRunning;
};

#endif

