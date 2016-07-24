#ifndef ZeldaImageProcessor_1468349491158922
#define ZeldaImageProcessor_1468349491158922

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

#include "ImageHandler.h"

class ZeldaImageProcessor {
 public:
  ZeldaImageProcessor();
  void UpdateData();
 private:
  bool FindZeldaScreen();
  std::vector<int> BoundingBox(std::vector<std::pair<int,int>> coordinates); //returns topleftx, toplefty, width, height
  std::vector<std::vector<std::pair<int,int>>> ConnectedComponents(std::vector<std::pair<int,int>> coordinates);

  ImageHandler GetScreen(); //returns a cropped screenshot that captures the zelda playing screen
  void RecordDoors(ImageHandler& screen, int mapx, int mapy);
  void RecordDarkDoors(ImageHandler& screen, int mapx, int mapy);
  void RecordSecretCave(ImageHandler& screen, int mapx, int mapy);
  
  int topleftx;
  int toplefty;
  int screenWidth;
  int screenHeight;

  int WHITE_R = 255;
  int WHITE_G = 255;
  int WHITE_B = 255;

  int BLACK_R = 0;
  int BLACK_G = 0;
  int BLACK_B = 0;

  int REFERENCE_SCREEN_WIDTH;
  int REFERENCE_SCREEN_HEIGHT;

  int REFERENCE_REGISTRATION_SCREEN_C_XCOOR;
  int REFERENCE_REGISTRATION_SCREEN_C_YCOOR;
  int REFERENCE_REGISTRATION_SCREEN_G_XCOOR;
  int REFERENCE_REGISTRATION_SCREEN_G_YCOOR;

  int CAPTURED_REGISTRATION_SCREEN_WIDTH;
  int CAPTURED_REGISTRATION_SCREEN_HEIGHT;
  int CAPTURED_REGISTRATION_SCREEN_XCOOR;
  int CAPTURED_REGISTRATION_SCREEN_YCOOR;
  double CAPTURED_REGISTRATION_SCREEN_SIMILARITY_THRESHOLD = 0.80;

  double SCALE_X;
  double SCALE_Y;

  int REFERENCE_OVERWORLD_MINIMAP_XCOOR = 16;
  int REFERENCE_OVERWORLD_MINIMAP_YCOOR = 16;
  int REFERENCE_OVERWORLD_MINIMAP_WIDTH = 64;
  int REFERENCE_OVERWORLD_MINIMAP_HEIGHT = 32;

  int REFERENCE_OVERWORLD_MINIMAP_CURSOR_XCOOR = 1;
  int REFERENCE_OVERWORLD_MINIMAP_CURSOR_YCOOR = 0;
  int REFERENCE_OVERWORLD_MINIMAP_CURSOR_WIDTH = 3;
  int REFERENCE_OVERWORLD_MINIMAP_CURSOR_HEIGHT = 3;
  
  int REFERENCE_DUNGEON_MINIMAP_CURSOR_XCOOR = 2;
  int REFERENCE_DUNGEON_MINIMAP_CURSOR_YCOOR = 0;
  int REFERENCE_DUNGEON_MINIMAP_CURSOR_WIDTH = 3;
  int REFERENCE_DUNGEON_MINIMAP_CURSOR_HEIGHT = 3;

  int REFERENCE_PLAYING_SCREEN_XCOOR = 0;
  int REFERENCE_PLAYING_SCREEN_YCOOR = 56;
  int REFERENCE_PLAYING_SCREEN_WIDTH = 256;
  int REFERENCE_PLAYING_SCREEN_HEIGHT = 168;

  bool INITIALIZED_MINIMAP_GRAY = false;
  bool INITIALIZED_TUNIC_GREEN = false;
  bool INITIALIZED_CURRENT_TUNIC = false;
  
  int TUNIC_GREEN_R;
  int TUNIC_GREEN_G;
  int TUNIC_GREEN_B;

  int MINIMAP_GRAY_R;
  int MINIMAP_GRAY_G;
  int MINIMAP_GRAY_B;

  int CURRENT_TUNIC_R;
  int CURRENT_TUNIC_G;
  int CURRENT_TUNIC_B;

  int HEART_RED_R;
  int HEART_RED_G;
  int HEART_RED_B;

  int REFERENCE_HEART_RED_XCOOR = 193;
  int REFERENCE_HEART_RED_YCOOR = 16;
  int REFERENCE_HEART_RED_WIDTH = 2;
  int REFERENCE_HEART_RED_HEIGHT = 7;

  int START_BLUE_R;
  int START_BLUE_G;
  int START_BLUE_B;

  int REFERENCE_WHITE_SWORD_XCOOR = 124;
  int REFERENCE_WHITE_SWORD_YCOOR = 145;
  int REFERENCE_WHITE_SWORD_WIDTH = 7;
  int REFERENCE_WHITE_SWORD_HEIGHT =16;

  int REFERENCE_MAGICAL_SWORD_XCOOR = 124;
  int REFERENCE_MAGICAL_SWORD_YCOOR = 145;
  int REFERENCE_MAGICAL_SWORD_WIDTH = 8;
  int REFERENCE_MAGICAL_SWORD_HEIGHT =16;

  int REFERENCE_SHOP_OWNER_XCOOR = 120;
  int REFERENCE_SHOP_OWNER_YCOOR = 121;
  int REFERENCE_SHOP_OWNER_WIDTH = 16;
  int REFERENCE_SHOP_OWNER_HEIGHT = 16;
  
  int REFERENCE_POTION_SHOP_TEXT_XCOOR = 37;
  int REFERENCE_POTION_SHOP_TEXT_YCOOR = 90;
  int REFERENCE_POTION_SHOP_TEXT_WIDTH = 181;
  int REFERENCE_POTION_SHOP_TEXT_HEIGHT = 28;  

  int REFERENCE_SHOP_LEFT_ITEM_XCOOR = 92;
  int REFERENCE_SHOP_LEFT_ITEM_YCOOR = 145;
  int REFERENCE_SHOP_LEFT_ITEM_WIDTH = 8;
  int REFERENCE_SHOP_LEFT_ITEM_HEIGHT =16;

  int REFERENCE_SHOP_MIDDLE_ITEM_XCOOR = 124;
  int REFERENCE_SHOP_MIDDLE_ITEM_YCOOR = 145;
  int REFERENCE_SHOP_MIDDLE_ITEM_WIDTH = 8;
  int REFERENCE_SHOP_MIDDLE_ITEM_HEIGHT =16;

  int REFERENCE_SHOP_RIGHT_ITEM_XCOOR = 156;
  int REFERENCE_SHOP_RIGHT_ITEM_YCOOR = 145;
  int REFERENCE_SHOP_RIGHT_ITEM_WIDTH = 8;
  int REFERENCE_SHOP_RIGHT_ITEM_HEIGHT =16;

  int REFERENCE_ANYROAD_XCOOR = 80;
  int REFERENCE_ANYROAD_YCOOR = 152;
  int REFERENCE_ANYROAD_WIDTH = 16;
  int REFERENCE_ANYROAD_HEIGHT = 16;  

  int REFERENCE_START_BLUE_XCOOR = 123;
  int REFERENCE_START_BLUE_YCOOR = 20;
  int REFERENCE_START_BLUE_WIDTH = 2;
  int REFERENCE_START_BLUE_HEIGHT = 24;

  int REFERENCE_DUNGEON_NINE_XCOOR = 64;
  int REFERENCE_DUNGEON_NINE_YCOOR = 8;
  int REFERENCE_DUNGEON_NINE_WIDTH = 7;
  int REFERENCE_DUNGEON_NINE_HEIGHT = 7;
  double CAPTURED_DUNGEON_NINE_SIMILARITY_THRESHOLD = 0.80;  

  double CAPTURED_ANYROAD_SIMILARITY_THRESHOLD = 0.80;    
  double CAPTURED_SWORD_SIMILARITY_THRESHOLD = 0.80;    
  double CAPTURED_SHOP_ITEM_SIMILARITY_THRESHOLD = 0.80;  
  double SECRET_CAVE_BLACK_THRESHOLD = 0.4;
  double SHOP_OWNER_BLACK_THRESHOLD = 0.5;

  int REFERENCE_DUNGEON_DOOR_HANDLE_XCOOR = 10;
  int REFERENCE_DUNGEON_DOOR_HANDLE_YCOOR = 161;
  int REFERENCE_DUNGEON_DOOR_HANDLE_WIDTH = 6;
  int REFERENCE_DUNGEON_DOOR_HANDLE_HEIGHT = 7;
  double CAPTURED_DUNGEON_DOOR_HANDLE_SIMILARITY_THRESHOLD = 0.80;    

  int REFERENCE_LEFT_DUNGEON_DOOR_XCOOR = 12;
  int REFERENCE_LEFT_DUNGEON_DOOR_YCOOR = 128;
  int REFERENCE_LEFT_DUNGEON_DOOR_WIDTH = 20;
  int REFERENCE_LEFT_DUNGEON_DOOR_HEIGHT = 32;
  double CAPTURED_LEFT_DOOR_SIMILARITY_THRESHOLD = 0.80;
  double CAPTURED_LEFT_KEY_DOOR_SIMILARITY_THRESHOLD = 0.60;
  double CAPTURED_LEFT_SHUTTER_DOOR_SIMILARITY_THRESHOLD = 0.60;
  double CAPTURED_LEFT_BOMB_DOOR_SIMILARITY_THRESHOLD = 0.80;

  int REFERENCE_RIGHT_DUNGEON_DOOR_XCOOR = 224;
  int REFERENCE_RIGHT_DUNGEON_DOOR_YCOOR = 128;
  int REFERENCE_RIGHT_DUNGEON_DOOR_WIDTH = 20;
  int REFERENCE_RIGHT_DUNGEON_DOOR_HEIGHT = 32;
  double CAPTURED_RIGHT_DOOR_SIMILARITY_THRESHOLD = 0.80;
  double CAPTURED_RIGHT_KEY_DOOR_SIMILARITY_THRESHOLD = 0.60;
  double CAPTURED_RIGHT_SHUTTER_DOOR_SIMILARITY_THRESHOLD = 0.60;
  double CAPTURED_RIGHT_BOMB_DOOR_SIMILARITY_THRESHOLD = 0.80;

  int REFERENCE_TOP_DUNGEON_DOOR_XCOOR = 112;
  int REFERENCE_TOP_DUNGEON_DOOR_YCOOR = 68;
  int REFERENCE_TOP_DUNGEON_DOOR_WIDTH = 32;
  int REFERENCE_TOP_DUNGEON_DOOR_HEIGHT = 20;
  double CAPTURED_TOP_DOOR_SIMILARITY_THRESHOLD = 0.80;
  double CAPTURED_TOP_KEY_DOOR_SIMILARITY_THRESHOLD = 0.60;
  double CAPTURED_TOP_SHUTTER_DOOR_SIMILARITY_THRESHOLD = 0.60;
  double CAPTURED_TOP_BOMB_DOOR_SIMILARITY_THRESHOLD = 0.80;
  
  int REFERENCE_BOTTOM_DUNGEON_DOOR_XCOOR = 112;
  int REFERENCE_BOTTOM_DUNGEON_DOOR_YCOOR = 200;
  int REFERENCE_BOTTOM_DUNGEON_DOOR_WIDTH = 32;
  int REFERENCE_BOTTOM_DUNGEON_DOOR_HEIGHT = 20;
  double CAPTURED_BOTTOM_DOOR_SIMILARITY_THRESHOLD = 0.80;
  double CAPTURED_BOTTOM_KEY_DOOR_SIMILARITY_THRESHOLD = 0.60;
  double CAPTURED_BOTTOM_SHUTTER_DOOR_SIMILARITY_THRESHOLD = 0.60;
  double CAPTURED_BOTTOM_BOMB_DOOR_SIMILARITY_THRESHOLD = 0.80;

  double CAPTURED_DUNGEON_ROOM_BLACK_THRESHOLD = 0.4;
  double CAPTURED_DUNGEON_DARK_ROOM_SIMILARITY_THRESHOLD = 0.85;
  double CAPTURED_DUNGEON_DARK_BOMB_ROOM_SIMILARITY_THRESHOLD = 0.95;

  int REFERENCE_DUNGEON_ITEM_XCOOR = 129;
  int REFERENCE_DUNGEON_ITEM_YCOOR = 137;
  int REFERENCE_DUNGEON_ITEM_WIDTH = 14;
  int REFERENCE_DUNGEON_ITEM_HEIGHT =16;
  double CAPTURED_DUNGEON_ITEM_SIMILARITY_THRESHOLD = 0.80;  

  double DUNGEON_TRIFORCE_BLACK_THRESHOLD = 0.90;  
  int DUNGEON_TRIFORCE_WIDTH_THRESHOLD = 20;
  int DUNGEON_TRIFORCE_HEIGHT_THRESHOLD = 30;

  int REFERENCE_HEART_XCOOR = 176;
  int REFERENCE_HEART_YCOOR = 32;
  int REFERENCE_HEART_WIDTH = 7;
  int REFERENCE_HEART_HEIGHT = 8;
  int REFERENCE_HEART_X_SEPERATION = 8;
  int REFERENCE_HEART_Y_SEPERATION = 8;
  double CAPTURED_HEART_COLOR_THRESHOLD = 0.5;    

  ImageHandler arrow;
  ImageHandler bait;
  ImageHandler bomb;  
  ImageHandler bluering;
  ImageHandler bluecandle;
  ImageHandler woodsword;  
  ImageHandler whitesword;
  ImageHandler magicalsword;
  ImageHandler potion;
  ImageHandler anyroad;  
  ImageHandler dungeonnine;
  ImageHandler book;
  ImageHandler bow;
  ImageHandler ladder;
  ImageHandler magicalkey;
  ImageHandler raft;      
  ImageHandler recorder;
  ImageHandler redcandle;
  ImageHandler redring;
  ImageHandler silverarrow;
  ImageHandler wand;    

  //doors
  ImageHandler dungeonld;
  ImageHandler dungeonlk;
  ImageHandler dungeonls;
  ImageHandler dungeonlb;
  ImageHandler dungeonrd;
  ImageHandler dungeonrk;
  ImageHandler dungeonrs;
  ImageHandler dungeonrb;
  ImageHandler dungeontd;
  ImageHandler dungeontk;
  ImageHandler dungeonts;
  ImageHandler dungeontb;
  ImageHandler dungeonbd;
  ImageHandler dungeonbk;
  ImageHandler dungeonbs;
  ImageHandler dungeonbb;
  ImageHandler dungeondld;
  ImageHandler dungeondlk;
  ImageHandler dungeondls;
  ImageHandler dungeondlb;
  ImageHandler dungeondrd;
  ImageHandler dungeondrk;
  ImageHandler dungeondrs;
  ImageHandler dungeondrb;
  ImageHandler dungeondtd;
  ImageHandler dungeondtk;
  ImageHandler dungeondts;
  ImageHandler dungeondtb;
  ImageHandler dungeondbd;
  ImageHandler dungeondbk;
  ImageHandler dungeondbs;
  ImageHandler dungeondbb;  
  ImageHandler dungeondoorhandle;
  bool dungeondoortransition = false;
};

#endif
