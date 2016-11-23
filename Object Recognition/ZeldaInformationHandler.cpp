#include "ZeldaInformationHandler.h"

std::recursive_mutex ZeldaInformationHandler::dataMutex;

int ZeldaInformationHandler::mapx = -1;
int ZeldaInformationHandler::mapy = -1;
int ZeldaInformationHandler::dungeonx = -1;
int ZeldaInformationHandler::dungeony = -1;

std::map<std::pair<int,int>, ZeldaInformationHandler::Secrets> ZeldaInformationHandler::overworldSecrets;
bool ZeldaInformationHandler::isRunning = true;
bool ZeldaInformationHandler::isInDungeon = false;
bool ZeldaInformationHandler::isStaircase = false;
std::vector<Dungeon> ZeldaInformationHandler::dungeons;
std::vector<std::vector<std::vector<bool>>> ZeldaInformationHandler::dungeonShapes;

std::map<ZeldaInformationHandler::Secrets, GLuint> ZeldaInformationHandler::overworldTextures;
std::map<Dungeon::RoomType, GLuint> ZeldaInformationHandler::dungeonTextures;
std::map<Dungeon::DoorType, GLuint> ZeldaInformationHandler::doorTextures;
std::map<Dungeon::DungeonItems, GLuint> ZeldaInformationHandler::itemTextures;

bool ZeldaInformationHandler::zeldaScreenFound;
int ZeldaInformationHandler::hearts;

void ZeldaInformationHandler::Init() {
  std::lock_guard<std::recursive_mutex> guard(dataMutex);
  for (int a = 0; a < 8; ++a) {
    for (int b = 0; b < 16; ++b) {
      overworldSecrets[std::make_pair(a,b)] = Secrets::UNEXPLORED;
    }
  }

  int d1data[] = {0,0,1,1,1,0,0,0,
		  0,0,0,1,0,0,0,0,
		  0,0,1,1,1,0,0,0,
		  0,1,1,1,1,1,0,0,
		  0,0,0,1,0,1,1,0,
		  0,0,1,1,0,0,0,0,
		  0,0,0,0,0,0,0,0,
		  0,0,0,0,0,0,0,0};
  int d2data[] = {0,0,0,1,1,0,0,0,
		  0,0,1,1,1,1,0,0,
		  0,0,0,0,1,1,0,0,
		  0,0,0,0,1,1,0,0,
		  0,0,0,0,1,1,0,0,
		  0,0,0,0,1,1,0,0,
		  0,0,0,0,1,1,0,0,
		  0,0,0,1,1,0,0,0};
  int d3data[] = {0,0,0,1,1,0,0,0,
		  0,1,0,1,0,0,0,0,
		  0,1,1,1,1,1,0,0,
		  0,1,1,1,1,1,0,0,
		  0,0,0,1,0,1,0,0,
		  0,0,1,1,0,0,0,0,
		  0,0,0,0,0,0,0,0,
		  0,0,0,0,0,0,0,0};
  int d4data[] = {0,0,1,1,0,0,0,0,
		  0,0,0,1,1,0,0,0,
		  0,0,1,1,0,0,0,0,
		  0,0,1,0,0,0,0,0,
		  0,0,1,1,1,0,0,0,
		  0,0,1,1,0,0,0,0,
		  0,0,1,1,1,1,0,0,
		  0,0,1,1,1,1,0,0};
  int d5data[] = {0,0,0,0,1,1,0,0,
		  0,0,1,1,1,1,0,0,
		  0,0,0,1,1,1,0,0,
		  0,0,0,0,1,1,0,0,
		  0,0,1,0,0,1,0,0,
		  0,0,1,1,1,1,0,0,
		  0,0,1,1,1,1,0,0,
		  0,0,0,1,1,0,0,0};
  int d6data[] = {0,1,1,1,0,0,0,0,
		  0,1,0,1,0,0,0,0,
		  0,1,0,0,0,0,0,0,
		  0,1,0,0,0,0,0,0,
		  0,1,1,1,0,1,0,0,
		  0,1,1,0,0,1,1,0,
		  0,1,1,1,1,1,1,0,
		  0,0,1,1,1,1,0,0};
  int d7data[] = {0,1,1,1,0,0,0,0,
		  0,1,1,1,1,1,1,0,
		  0,1,1,1,1,0,0,0,
		  0,1,1,0,0,0,0,0,
		  0,1,1,1,0,0,0,0,
		  0,1,1,1,1,0,0,0,
		  0,1,1,1,1,1,0,0,
		  0,1,1,1,1,1,1,0};
  int d8data[] = {0,0,1,1,1,1,0,0,
		  0,0,0,0,1,0,0,0,
		  0,0,1,1,1,1,0,0,
		  0,1,1,1,1,0,0,0,
		  0,1,1,1,1,1,0,0,
		  0,0,1,1,1,0,0,0,
		  0,0,0,1,1,1,0,0,
		  0,0,0,0,1,0,0,0};
  int d9data[] = {0,1,0,1,1,0,1,0,
		  0,1,1,1,1,1,1,0,
		  1,1,1,1,1,1,1,1,
		  1,1,1,1,1,1,1,1,
		  1,1,1,1,1,1,1,1,
		  1,1,1,1,1,1,1,1,
		  1,1,1,1,1,1,1,1,
		  0,1,1,1,1,1,1,1};
		  
  dungeonShapes.push_back(FormatShape(d1data));
  dungeonShapes.push_back(FormatShape(d2data));
  dungeonShapes.push_back(FormatShape(d3data));
  dungeonShapes.push_back(FormatShape(d4data));
  dungeonShapes.push_back(FormatShape(d5data));
  dungeonShapes.push_back(FormatShape(d6data));
  dungeonShapes.push_back(FormatShape(d7data));
  dungeonShapes.push_back(FormatShape(d8data));
  dungeonShapes.push_back(FormatShape(d9data));

  int overworldDeadData[] = {0,0,1,1,0,0,0,0,0,0,1,0,0,0,1,1,
			     1,1,0,0,0,1,0,0,0,1,0,0,1,0,1,0,
			     1,0,1,1,1,1,0,1,1,1,1,0,1,1,0,1,
			     1,1,0,1,0,0,0,0,0,0,0,0,1,0,0,1,
			     1,1,1,0,0,1,1,0,1,1,1,1,0,0,1,1,
			     1,0,0,0,0,0,0,0,0,1,1,1,0,0,1,0,
			     0,1,0,0,0,1,0,1,1,1,0,1,0,0,0,0,
			     1,0,1,0,0,0,1,0,1,1,0,0,0,0,0,0};
  for (int x = 0; x < 16; ++x) {
    for (int y = 0; y < 8; ++y) {
      if (overworldDeadData[x + 16*(7-y)]) {
	overworldSecrets[std::make_pair(x,y)] = Secrets::UNKNOWN_CAVE;
      }
      else {
	overworldSecrets[std::make_pair(x,y)] = Secrets::UNEXPLORED;
      }
    }
  }

  hearts = 0;
}

void ZeldaInformationHandler::InitTextures() {
  OpenGLUtility::Load2DTexture("Images/Selectors/OverworldDungeon3.png", GL_RGBA);
  overworldTextures[Secrets::UNKNOWN_CAVE] = OpenGLUtility::Load2DTexture("Images/Selectors/OverworldExplored.png", GL_RGBA);
  overworldTextures[Secrets::EXPLORED_CAVE] = OpenGLUtility::Load2DTexture("Images/Selectors/OverworldExplored.png", GL_RGBA);  
  overworldTextures[Secrets::UNKNOWN_DUNGEON] = OpenGLUtility::Load2DTexture("Images/Selectors/OverworldUnknownDungeon.png", GL_RGBA);
  overworldTextures[Secrets::DUNGEON_1] = OpenGLUtility::Load2DTexture("Images/Selectors/OverworldDungeon1.png", GL_RGBA);
  overworldTextures[Secrets::DUNGEON_2] = OpenGLUtility::Load2DTexture("Images/Selectors/OverworldDungeon2.png", GL_RGBA);
  overworldTextures[Secrets::DUNGEON_3] = OpenGLUtility::Load2DTexture("Images/Selectors/OverworldDungeon3.png", GL_RGBA);
  overworldTextures[Secrets::DUNGEON_4] = OpenGLUtility::Load2DTexture("Images/Selectors/OverworldDungeon4.png", GL_RGBA);
  overworldTextures[Secrets::DUNGEON_5] = OpenGLUtility::Load2DTexture("Images/Selectors/OverworldDungeon5.png", GL_RGBA);
  overworldTextures[Secrets::DUNGEON_6] = OpenGLUtility::Load2DTexture("Images/Selectors/OverworldDungeon6.png", GL_RGBA);
  overworldTextures[Secrets::DUNGEON_7] = OpenGLUtility::Load2DTexture("Images/Selectors/OverworldDungeon7.png", GL_RGBA);
  overworldTextures[Secrets::DUNGEON_8] = OpenGLUtility::Load2DTexture("Images/Selectors/OverworldDungeon8.png", GL_RGBA);
  overworldTextures[Secrets::DUNGEON_9] = OpenGLUtility::Load2DTexture("Images/Selectors/OverworldDungeon9.png", GL_RGBA);
  overworldTextures[Secrets::ARROW_SHOP] = OpenGLUtility::Load2DTexture("Images/Selectors/ArrowShop.png", GL_RGBA);
  overworldTextures[Secrets::BAIT_SHOP] = OpenGLUtility::Load2DTexture("Images/Selectors/BaitShop.png", GL_RGBA);
  overworldTextures[Secrets::BOMB_SHOP] = OpenGLUtility::Load2DTexture("Images/Selectors/BombShop.png", GL_RGBA);  
  overworldTextures[Secrets::CANDLE_SHOP] = OpenGLUtility::Load2DTexture("Images/Selectors/CandleShop.png", GL_RGBA);
  overworldTextures[Secrets::BLUE_RING_SHOP] = OpenGLUtility::Load2DTexture("Images/Selectors/BlueRingShop.png", GL_RGBA);
  overworldTextures[Secrets::ANYROAD] = OpenGLUtility::Load2DTexture("Images/Selectors/Anyroad.png", GL_RGBA);          
  overworldTextures[Secrets::PRE_POTION_SHOP] = OpenGLUtility::Load2DTexture("Images/Selectors/PotionShop.png", GL_RGBA);
  overworldTextures[Secrets::POTION_SHOP] = OpenGLUtility::Load2DTexture("Images/Selectors/PotionShop.png", GL_RGBA); 
  overworldTextures[Secrets::WOOD_SWORD] = OpenGLUtility::Load2DTexture("Images/Selectors/WoodSword.png", GL_RGBA); 
  overworldTextures[Secrets::WHITE_SWORD] = OpenGLUtility::Load2DTexture("Images/Selectors/WhiteSword.png", GL_RGBA);
  overworldTextures[Secrets::MAGICAL_SWORD] = OpenGLUtility::Load2DTexture("Images/Selectors/MagicalSword.png", GL_RGBA);
  overworldTextures[Secrets::BONUS_CAVE] = OpenGLUtility::Load2DTexture("Images/Selectors/OverworldExplored.png", GL_RGBA);  
      
  dungeonTextures[Dungeon::RoomType::UNEXPLORED] = OpenGLUtility::Load2DTexture("Images/Selectors/Transparent.png", GL_RGBA);
  dungeonTextures[Dungeon::RoomType::UNKNOWN_ROOM] = OpenGLUtility::Load2DTexture("Images/Selectors/DungeonRoom.png", GL_RGBA);
  dungeonTextures[Dungeon::RoomType::UNSEEN_ROOM] = OpenGLUtility::Load2DTexture("Images/Selectors/DungeonUnseenRoom.png", GL_RGBA);
  dungeonTextures[Dungeon::RoomType::GUESS_ROOM] = OpenGLUtility::Load2DTexture("Images/Selectors/DungeonUnseenRoom.png", GL_RGBA);
  dungeonTextures[Dungeon::RoomType::TRIFORCE_ROOM] = OpenGLUtility::Load2DTexture("Images/Selectors/DungeonTriforceRoom.png", GL_RGBA);
  dungeonTextures[Dungeon::RoomType::STAIRCASE_1] = OpenGLUtility::Load2DTexture("Images/Selectors/Staircase1.png", GL_RGBA);
  dungeonTextures[Dungeon::RoomType::STAIRCASE_2] = OpenGLUtility::Load2DTexture("Images/Selectors/Staircase2.png", GL_RGBA);
  dungeonTextures[Dungeon::RoomType::STAIRCASE_3] = OpenGLUtility::Load2DTexture("Images/Selectors/Staircase3.png", GL_RGBA);
  dungeonTextures[Dungeon::RoomType::STAIRCASE_4] = OpenGLUtility::Load2DTexture("Images/Selectors/Staircase4.png", GL_RGBA);
  dungeonTextures[Dungeon::RoomType::STAIRCASE_5] = OpenGLUtility::Load2DTexture("Images/Selectors/Staircase5.png", GL_RGBA);
  dungeonTextures[Dungeon::RoomType::STAIRCASE_6] = OpenGLUtility::Load2DTexture("Images/Selectors/Staircase6.png", GL_RGBA);
  dungeonTextures[Dungeon::RoomType::STAIRCASE_7] = OpenGLUtility::Load2DTexture("Images/Selectors/Staircase7.png", GL_RGBA);    

  doorTextures[Dungeon::DoorType::UNEXPLORED] = OpenGLUtility::Load2DTexture("Images/Selectors/Transparent.png", GL_RGBA);
  doorTextures[Dungeon::DoorType::OPEN] = OpenGLUtility::Load2DTexture("Images/Selectors/DungeonOpenDoor.png", GL_RGBA);
  doorTextures[Dungeon::DoorType::SHUTTER] = OpenGLUtility::Load2DTexture("Images/Selectors/DungeonShutterDoor.png", GL_RGBA);
  doorTextures[Dungeon::DoorType::KEY] = OpenGLUtility::Load2DTexture("Images/Selectors/DungeonKeyDoor.png", GL_RGBA);
  doorTextures[Dungeon::DoorType::BOMB] = OpenGLUtility::Load2DTexture("Images/Selectors/DungeonBombDoor.png", GL_RGBA);

  itemTextures[Dungeon::DungeonItems::NONE] = OpenGLUtility::Load2DTexture("Images/Selectors/PreItem.png", GL_RGBA);
  itemTextures[Dungeon::DungeonItems::BOOK] = OpenGLUtility::Load2DTexture("Images/Selectors/Book.png", GL_RGBA);
  itemTextures[Dungeon::DungeonItems::BOW] = OpenGLUtility::Load2DTexture("Images/Selectors/Bow.png", GL_RGBA);
  itemTextures[Dungeon::DungeonItems::HEART_CONTAINER] = OpenGLUtility::Load2DTexture("Images/Selectors/HeartContainer.png", GL_RGBA);
  itemTextures[Dungeon::DungeonItems::LADDER] = OpenGLUtility::Load2DTexture("Images/Selectors/Ladder.png", GL_RGBA);
  itemTextures[Dungeon::DungeonItems::MAGICAL_BOOMERANG] = OpenGLUtility::Load2DTexture("Images/Selectors/MagicalBoomerang.png", GL_RGBA);
  itemTextures[Dungeon::DungeonItems::WOODEN_BOOMERANG] = OpenGLUtility::Load2DTexture("Images/Selectors/WoodenBoomerang.png", GL_RGBA);  
  itemTextures[Dungeon::DungeonItems::MAGICAL_KEY] = OpenGLUtility::Load2DTexture("Images/Selectors/MagicalKey.png", GL_RGBA);
  itemTextures[Dungeon::DungeonItems::RAFT] = OpenGLUtility::Load2DTexture("Images/Selectors/Raft.png", GL_RGBA);  
  itemTextures[Dungeon::DungeonItems::RECORDER] = OpenGLUtility::Load2DTexture("Images/Selectors/Recorder.png", GL_RGBA);
  itemTextures[Dungeon::DungeonItems::RED_CANDLE] = OpenGLUtility::Load2DTexture("Images/Selectors/RedCandle.png", GL_RGBA);
  itemTextures[Dungeon::DungeonItems::RED_RING] = OpenGLUtility::Load2DTexture("Images/Selectors/RedRing.png", GL_RGBA);
  itemTextures[Dungeon::DungeonItems::POWER_BRACELET] = OpenGLUtility::Load2DTexture("Images/Selectors/PowerBracelet.png", GL_RGBA);  
  itemTextures[Dungeon::DungeonItems::SILVER_ARROW] = OpenGLUtility::Load2DTexture("Images/Selectors/SilverArrow.png", GL_RGBA);
  itemTextures[Dungeon::DungeonItems::WAND] = OpenGLUtility::Load2DTexture("Images/Selectors/Wand.png", GL_RGBA);
  itemTextures[Dungeon::DungeonItems::WHITE_SWORD] = OpenGLUtility::Load2DTexture("Images/Selectors/WhiteSwordItem.png", GL_RGBA);  
  itemTextures[Dungeon::DungeonItems::WOODEN_BOOMERANG] = OpenGLUtility::Load2DTexture("Images/Selectors/WoodenBoomerang.png", GL_RGBA);  
}

std::vector<std::vector<bool>> ZeldaInformationHandler::FormatShape(int shape[]) {
  std::vector<std::vector<bool>> format;
  for (int y = 0; y < 8; ++y) {
    std::vector<bool> row;
    for (int x = 0; x < 8; ++x) {
      int bit = *shape++;
      if (bit == 0) {
	row.push_back(false);
      }
      else {
	row.push_back(true);
      }
    }
    format.push_back(row);
  }
  std::reverse(std::begin(format), std::end(format));
  std::vector<std::vector<bool>> reformat;
  for (int x = 0; x < 8; ++x) {
    std::vector<bool> col;
    for (int y = 0; y < 8; ++y) {
      col.push_back(format[y][x]);
    }
    reformat.push_back(col);
  }
  return reformat;
}

void ZeldaInformationHandler::SetMapLocation(int x, int y) {
  std::lock_guard<std::recursive_mutex> guard(dataMutex);
  SetIsInDungeon(false);
  isStaircase = false;
  mapx = x;
  mapy = y;
}


std::pair<int, int> ZeldaInformationHandler::GetMapLocation() {
  std::lock_guard<std::recursive_mutex> guard(dataMutex);
  return std::make_pair(mapx, mapy);
}

void ZeldaInformationHandler::SetDungeonLocation(int x, int y, Dungeon::RoomType type) {
  std::lock_guard<std::recursive_mutex> guard(dataMutex);
  SetIsInDungeon(true);
  std::pair<int, int> prevloc = GetDungeonLocation();
  std::pair<int, int> loc = GetMapLocation();
  bool found = false;
  Secrets number;
  for (auto& el : dungeons) {
    if (el.GetLocation() == loc) {
      el.SetLocation(x, y, type);
      found = true;
      number = AsSecret(el.Number());
    }
  }
  if (!found) {
    Dungeon dungeon(loc.first,loc.second);
    dungeon.SetLocation(x, y, type);
    dungeons.push_back(dungeon);
    number = AsSecret(dungeon.Number());
  }
  SetSecret(loc.first, loc.second, number);
  if (type != Dungeon::RoomType::UNSEEN_ROOM && type != Dungeon::RoomType::TRIFORCE_ROOM) {
    dungeonx = x;
    dungeony = y;
  }
  if (isStaircase) {
    isStaircase = false;
    for (auto& el : dungeons) {
      if (el.GetLocation() == loc) {
	if (prevloc != el.GetStartLocation() && std::make_pair(x,y) != el.GetStartLocation() && prevloc != std::make_pair(x,y)) {
	  el.SetStaircase(prevloc, std::make_pair(x,y));
	}
      }
    }    
  }  
}


std::pair<int, int> ZeldaInformationHandler::GetDungeonLocation() {
  std::lock_guard<std::recursive_mutex> guard(dataMutex);
  return std::make_pair(dungeonx, dungeony);
}

bool ZeldaInformationHandler::GetTriforce(int level) {
  std::lock_guard<std::recursive_mutex> guard(dataMutex);
  Secrets levelNumber = AsSecret(level);
  for (auto& el : dungeons) {
    if (AsSecret(el.Number()) == levelNumber) {
      return el.GetTriforce();
    }
  }  
  return false;
}

void ZeldaInformationHandler::SetStaircase() {
  isStaircase = true;
}

void ZeldaInformationHandler::SetTriforce() {
  std::lock_guard<std::recursive_mutex> guard(dataMutex);
  std::pair<int, int> loc = GetMapLocation();
  for (auto& el : dungeons) {
    if (el.GetLocation() == loc) {
      el.SetTriforce();
    }
  }
}

ZeldaInformationHandler::Secrets ZeldaInformationHandler::GetSecret(int x, int y) {
  std::lock_guard<std::recursive_mutex> guard(dataMutex);
  return overworldSecrets[std::make_pair(x,y)];
}

void ZeldaInformationHandler::SetSecret(int x, int y, Secrets secret) {
  std::lock_guard<std::recursive_mutex> guard(dataMutex);
  bool set = true;
  //prevent overriding with less specific data
  Secrets prev = GetSecret(x, y);  
  if ((secret == Secrets::UNKNOWN_CAVE) || (secret == Secrets::EXPLORED_CAVE)) {
    if (prev == Secrets::ARROW_SHOP || prev == Secrets::BAIT_SHOP || prev == Secrets::CANDLE_SHOP || prev == Secrets::BLUE_RING_SHOP
	|| prev == Secrets::POTION_SHOP || prev == Secrets::BOMB_SHOP || prev == Secrets::WHITE_SWORD || prev == Secrets::MAGICAL_SWORD
	|| prev == Secrets::BONUS_CAVE || prev == Secrets::ANYROAD || prev == Secrets::WOOD_SWORD) {
      set = false;
    }
  }
  //Potion shops look different (like a bonus cave) after you take a potion
  if (prev == Secrets::POTION_SHOP && secret == Secrets::BONUS_CAVE) {
    set = false;
  }
  //because bomb shops are least important
  if (secret == Secrets::BOMB_SHOP) {
    if(prev == Secrets::ARROW_SHOP || prev == Secrets::BAIT_SHOP || prev == Secrets::CANDLE_SHOP || prev == Secrets::BLUE_RING_SHOP) {
      set = false;
    }
  }
  //because the blue ring shop can be hidden by other shops
  if (prev == Secrets::BLUE_RING_SHOP) {
    if (secret == Secrets::ARROW_SHOP || secret == Secrets::BAIT_SHOP || secret == Secrets::CANDLE_SHOP || secret == Secrets::BOMB_SHOP) {
      set = false;
    }
  }
  //because bonus caves can have candles
  if (prev == Secrets::BONUS_CAVE && secret == Secrets::CANDLE_SHOP) {
    set = false;
  }
  //because wooden sword cave can have candle
  if (prev == Secrets::WOOD_SWORD && secret == Secrets::CANDLE_SHOP) {
    set = false;
  }
  //because swords look alike
  if (prev == Secrets::WHITE_SWORD && secret == Secrets::WOOD_SWORD) {
    set = false;
  }
  if (prev == Secrets::MAGICAL_SWORD && secret == Secrets::WHITE_SWORD) {
    set = false;
  }    
  if (secret == Secrets::UNKNOWN_CAVE) {
    if (prev == Secrets::EXPLORED_CAVE) {
      set = false;
    }
  }
  //if seeing level nine for the first time
  if (secret == Secrets::DUNGEON_9 && prev != Secrets::DUNGEON_9) {
    Dungeon dungeon(x, y);
    dungeon.SetLevelNine();
    dungeons.push_back(dungeon);
  }
  if (set) {
    overworldSecrets[std::make_pair(x,y)] = secret;
  }
}

void ZeldaInformationHandler::SetIsRunning(bool running) {
  std::lock_guard<std::recursive_mutex> guard(dataMutex);
  isRunning = running;
}

bool ZeldaInformationHandler::GetIsRunning() {
  std::lock_guard<std::recursive_mutex> guard(dataMutex);
  return isRunning;
}

void ZeldaInformationHandler::SetHearts(int numHearts) {
  std::lock_guard<std::recursive_mutex> guard(dataMutex);
  if (numHearts > 0 && numHearts > hearts && (numHearts <= hearts+1 || hearts == 0)) {
    hearts = numHearts;
    if (GetIsInDungeon()) {
      std::pair<int, int> loc = GetMapLocation();
      for (auto& el : dungeons) {
	if (el.GetLocation() == loc) {
	  std::pair<int, int> dloc = GetDungeonLocation();
	  if (el.GetItem(dloc) != Dungeon::DungeonItems::HEART_CONTAINER) {
	    el.SetHeart();
	  }
	}
      }
    }
  }  
}

int ZeldaInformationHandler::GetHearts() {
  std::lock_guard<std::recursive_mutex> guard(dataMutex);
  return hearts;
}

bool ZeldaInformationHandler::GetHeart(int level) {
  std::lock_guard<std::recursive_mutex> guard(dataMutex);
  Secrets levelNumber = AsSecret(level);
  for (auto& el : dungeons) {
    if (AsSecret(el.Number()) == levelNumber) {
      return el.GetHeart();
    }
  }  
  return false;
}

int ZeldaInformationHandler::GetBonusCaves() {
  std::lock_guard<std::recursive_mutex> guard(dataMutex);
  int caves = 0;
  for (int x = 0; x < 16; ++x) {
    for (int y = 0; y < 8; ++y) {
      if (overworldSecrets[std::make_pair(x,y)] == Secrets::BONUS_CAVE) {
	caves++;
      }
    }
  }
  return caves;
}

void ZeldaInformationHandler::SetIsInDungeon(bool inDungeon) {
  std::lock_guard<std::recursive_mutex> guard(dataMutex);
  isInDungeon = inDungeon;
}

Dungeon::RoomType ZeldaInformationHandler::GetDungeonRoomType(int x, int y) {
  std::lock_guard<std::recursive_mutex> guard(dataMutex);
  std::pair<int, int> loc = GetMapLocation();
  for (auto& el : dungeons) {
    if (el.GetLocation() == loc) {
      return el.GetRoomType(x, y);
    }
  }
  return Dungeon::RoomType::UNEXPLORED;
}

bool ZeldaInformationHandler::GetIsInDungeon() {
  std::lock_guard<std::recursive_mutex> guard(dataMutex);
  return isInDungeon;
}

void ZeldaInformationHandler::SetZeldaSceenFound(bool found) {
  std::lock_guard<std::recursive_mutex> guard(dataMutex);
  zeldaScreenFound = found;
}

bool ZeldaInformationHandler::GetZeldaSceenFound() {
  std::lock_guard<std::recursive_mutex> guard(dataMutex);
  return zeldaScreenFound;
}

void ZeldaInformationHandler::SetItem(Dungeon::DungeonItems item) {
  std::lock_guard<std::recursive_mutex> guard(dataMutex);
  std::pair<int, int> loc = GetMapLocation();
  for (auto& el : dungeons) {
    if (el.GetLocation() == loc) {
      el.SetItem(item, GetDungeonLocation());
    }
  }
}

Dungeon::DungeonItems ZeldaInformationHandler::GetItem(int level, bool first) {
  std::lock_guard<std::recursive_mutex> guard(dataMutex);
  Secrets levelNumber = AsSecret(level);
  for (auto& el : dungeons) {
    if (AsSecret(el.Number()) == levelNumber) {
      if (first) {
	return el.GetFirstItem();
      }
      else {
	return el.GetSecondItem();
      }
    }
  }
  return Dungeon::DungeonItems::NONE;
}

Dungeon::DoorType ZeldaInformationHandler::GetDungeonDoor(int x1, int y1, int x2, int y2) {
  std::lock_guard<std::recursive_mutex> guard(dataMutex);
  std::pair<int, int> loc = GetMapLocation();
  for (auto& el : dungeons) {
    if (el.GetLocation() == loc) {
      return el.GetDoorType(x1, y1, x2, y2);
    }
  }
  return Dungeon::DoorType::UNEXPLORED;  
}

void ZeldaInformationHandler::SetDungeonDoor(int x1, int y1, int x2, int y2, Dungeon::DoorType type) {
  std::lock_guard<std::recursive_mutex> guard(dataMutex);
  std::pair<int, int> loc = GetMapLocation();
  for (auto& el : dungeons) {
    if (el.GetLocation() == loc) {
      el.SetDoorType(x1, y1, x2, y2, type);
    }
  }
}

ZeldaInformationHandler::Secrets ZeldaInformationHandler::AsSecret(Dungeon::DungeonType type) {
  switch(type) {
  case Dungeon::DungeonType::UNKNOWN_DUNGEON:
    return Secrets::UNKNOWN_DUNGEON;
  case Dungeon::DungeonType::DUNGEON_1:
    return Secrets::DUNGEON_1;
  case Dungeon::DungeonType::DUNGEON_2:
    return Secrets::DUNGEON_2;
  case Dungeon::DungeonType::DUNGEON_3:
    return Secrets::DUNGEON_3;
  case Dungeon::DungeonType::DUNGEON_4:
    return Secrets::DUNGEON_4;
  case Dungeon::DungeonType::DUNGEON_5:
    return Secrets::DUNGEON_5;
  case Dungeon::DungeonType::DUNGEON_6:
    return Secrets::DUNGEON_6;
  case Dungeon::DungeonType::DUNGEON_7:
    return Secrets::DUNGEON_7;
  case Dungeon::DungeonType::DUNGEON_8:
    return Secrets::DUNGEON_8;
  case Dungeon::DungeonType::DUNGEON_9:
    return Secrets::DUNGEON_9;
  }
}

ZeldaInformationHandler::Secrets ZeldaInformationHandler::AsSecret(int dungeonNumber) {
  switch(dungeonNumber) {
  case 0:
    return Secrets::DUNGEON_1;
  case 1:
    return Secrets::DUNGEON_2;
  case 2:
    return Secrets::DUNGEON_3;
  case 3:
    return Secrets::DUNGEON_4;
  case 4:
    return Secrets::DUNGEON_5;
  case 5:
    return Secrets::DUNGEON_6;
  case 6:
    return Secrets::DUNGEON_7;
  case 7:
    return Secrets::DUNGEON_8;
  case 8:
    return Secrets::DUNGEON_9;
  default:
    return Secrets::UNKNOWN_DUNGEON;
  }
}

GLuint ZeldaInformationHandler::GetTexture(Secrets type) {
  return overworldTextures[type];
}

GLuint ZeldaInformationHandler::GetTexture(Dungeon::RoomType type) {
  return dungeonTextures[type];
}

GLuint ZeldaInformationHandler::GetTexture(Dungeon::DoorType type) {
  return doorTextures[type];
}

GLuint ZeldaInformationHandler::GetTexture(Dungeon::DungeonItems type) {
  return itemTextures[type];
}
