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

class ZeldaInformationHandler {
 public:
  enum class Secrets {UNEXPLORED, UNKNOWN_CAVE, UNKNOWN_DUNGEON, DUNGEON_1, DUNGEON_2,
      DUNGEON_3, DUNGEON_4, DUNGEON_5, DUNGEON_6, DUNGEON_7, DUNGEON_8, DUNGEON_9,
      CANDLE_SHOP, BAIT_SHOP, BOMB_SHOP, ARROW_SHOP, BLUE_RING_SHOP, WOOD_SWORD, WHITE_SWORD,
      MAGICAL_SWORD, PRE_POTION_SHOP, POTION_SHOP, EXPLORED_CAVE, BONUS_CAVE, ANYROAD};
  enum class RoomType {UNEXPLORED, UNKNOWN_ROOM, UNSEEN_ROOM, GUESS_ROOM, TRIFORCE_ROOM};
  enum class DoorType {UNEXPLORED, OPEN, KEY, SHUTTER, BOMB};
  enum class DungeonItems {BOOK, BOW, HEART_CONTAINER, LADDER, MAGICAL_BOOMERANG, MAGICAL_KEY, NONE, RAFT, RECORDER, RED_CANDLE,
      RED_RING, POWER_BRACELET, SILVER_ARROW, WAND, WHITE_SWORD, WOODEN_BOOMERANG};  

  static void Init();
  static void InitTextures();
  static void SetZeldaSceenFound(bool found);
  static bool GetZeldaSceenFound();  
  static void SetMapLocation(int x, int y);
  static std::pair<int, int> GetMapLocation();
  static std::pair<int, int> GetDungeonLocation();
  static void SetDungeonLocation(int x, int y, RoomType type);
  static RoomType GetDungeonRoomType(int x, int y);
  static DoorType GetDungeonDoor(int x1, int y1, int x2, int y2);
  static void SetDungeonDoor(int x1, int y1, int x2, int y2, DoorType type);
  static void SetIsRunning(bool running);
  static bool GetIsRunning();
  static bool GetIsInDungeon();
  static Secrets GetSecret(int x, int y);
  static void SetSecret(int x, int y, Secrets secret);
  static GLuint GetTexture(Secrets type);
  static GLuint GetTexture(RoomType type);
  static GLuint GetTexture(DoorType type);
  static GLuint GetTexture(DungeonItems type);  
  static bool GetTriforce(int level); //zero indexed
  static void SetTriforce();
  static void SetHearts(int numHearts);
  static bool GetHeart(int level); //zero indexed  
  static int GetHearts();
  static int GetBonusCaves();  
  static void SetItem(DungeonItems item);
  static DungeonItems GetItem(int level, bool first);  
 private:
  static std::recursive_mutex dataMutex;

  static bool zeldaScreenFound;

  static std::map<Secrets, GLuint> overworldTextures;
  static std::map<RoomType, GLuint> dungeonTextures;
  static std::map<DoorType, GLuint> doorTextures;
  static std::map<DungeonItems, GLuint> itemTextures;    

  static int mapx;
  static int mapy;
  static int dungeonx;
  static int dungeony;
  
  class Dungeon {
  public:
    Dungeon(int overworldx, int overworldy);
    void SetLevelNine();
    void SetLocation(int x, int y, RoomType type);
    void SetDoorType(int x1, int y1, int x2, int y2, DoorType type);
    DoorType GetDoorType(int x1, int y1, int x2, int y2);    
    RoomType GetRoomType(int x, int y);
    std::pair<int, int> GetLocation();
    Secrets Number();
    void SetTriforce();
    bool GetTriforce();
    void SetHeart();
    bool GetHeart();
    void SetItem(DungeonItems item);
    DungeonItems GetFirstItem();
    DungeonItems GetSecondItem();
    DungeonItems GetItem(std::pair<int, int> loc);            
  private:
    int overworldx;
    int overworldy;
    Secrets levelNumber;
    bool triforce;
    bool heart;
    DungeonItems firstItem;
    std::pair<int, int> firstitemloc;
    DungeonItems secondItem;
    std::pair<int, int> seconditemloc;    
    std::map<std::pair<int, int>, RoomType> rooms;
    std::map<std::tuple<int, int, int, int>, DoorType> doors;
  };
  static std::vector<Dungeon> dungeons;
  static std::vector<std::vector<std::vector<bool>>> dungeonShapes;
  static std::vector<std::vector<bool>> FormatShape(int shape[]);
  
  static void SetIsInDungeon(bool isInDungeon);
  static bool isInDungeon;
  static std::map<std::pair<int,int>, Secrets> overworldSecrets;
  static int hearts;
  
  static bool isRunning;
};

#endif

