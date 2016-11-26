#include "ZeldaInformationHandler.h"

#include "RandomDungeonShapeHandler.h"
#include "FirstQuestDungeonHandler.h"
#include "SecondQuestDungeonHandler.h"

std::recursive_mutex ZeldaInformationHandler::dataMutex;

int ZeldaInformationHandler::mapx = -1;
int ZeldaInformationHandler::mapy = -1;
int ZeldaInformationHandler::dungeonx = -1;
int ZeldaInformationHandler::dungeony = -1;

bool ZeldaInformationHandler::isRunning = true;
bool ZeldaInformationHandler::isInDungeon = false;
bool ZeldaInformationHandler::isStaircase = false;
std::vector<Dungeon> ZeldaInformationHandler::dungeons;
std::shared_ptr<DungeonHandler> ZeldaInformationHandler::dungeonHandler;

struct ZeldaInformationHandler::EnumHasher {
  template <typename T>
  std::size_t operator()(T t) const {
    return static_cast<std::size_t>(t);
  }
};
struct ZeldaInformationHandler::PairHasher {
  std::size_t operator()(std::pair<int,int> t) const {
    std::size_t hash = 1;
    hash += 31 * t.first;
    hash += 31 * t.second;
    return hash;
  }
};
std::unordered_map<ZeldaInformationHandler::Secrets, GLuint, ZeldaInformationHandler::EnumHasher> ZeldaInformationHandler::overworldTextures;
std::unordered_map<Dungeon::RoomType, GLuint, ZeldaInformationHandler::EnumHasher> ZeldaInformationHandler::dungeonTextures;
std::unordered_map<Dungeon::DoorType, GLuint, ZeldaInformationHandler::EnumHasher> ZeldaInformationHandler::doorTextures;
std::unordered_map<Dungeon::DungeonItems, GLuint, ZeldaInformationHandler::EnumHasher> ZeldaInformationHandler::itemTextures;
std::unordered_map<std::pair<int,int>, ZeldaInformationHandler::Secrets, ZeldaInformationHandler::PairHasher> ZeldaInformationHandler::overworldSecrets;

bool ZeldaInformationHandler::zeldaScreenFound;
int ZeldaInformationHandler::hearts = 0;
bool ZeldaInformationHandler::optionsInitialized = false;
bool ZeldaInformationHandler::firstQuest = true;

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

bool ZeldaInformationHandler::GetQuest() {
  std::lock_guard<std::recursive_mutex> guard(dataMutex);
  return firstQuest;
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
  for (auto& el : dungeons) {
    if (el.GetLocation() == loc) {
      dungeonHandler->PredictLevel(el);
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
  //if seeing a level for the first time
  if ((secret == Secrets::DUNGEON_1 || secret == Secrets::DUNGEON_2 || secret == Secrets::DUNGEON_3 ||
       secret == Secrets::DUNGEON_4 || secret == Secrets::DUNGEON_5 || secret == Secrets::DUNGEON_6 ||
       secret == Secrets::DUNGEON_7 || secret == Secrets::DUNGEON_8 || secret == Secrets::DUNGEON_9)
       && prev != secret) {
    bool found = false;
    for (auto& el : dungeons) {
      if (el.GetLocation() == std::make_pair(x,y)) {
	el.SetDungeonType(AsDungeonType(secret));
	found = true;
      }
    }
    if (!found) {
      Dungeon dungeon(x, y);
      dungeon.SetDungeonType(AsDungeonType(secret));
      dungeons.push_back(dungeon);
    }
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

bool ZeldaInformationHandler::GetIsOptionsInitialized() {
  std::lock_guard<std::recursive_mutex> guard(dataMutex);
  return optionsInitialized;
}

void ZeldaInformationHandler::SetOptions(bool quest, bool randomDungeonShapes) {
  std::lock_guard<std::recursive_mutex> guard(dataMutex);
  optionsInitialized = true;
  firstQuest = quest;
  if (randomDungeonShapes) {
    dungeonHandler = std::make_shared<RandomDungeonShapeHandler>();
  }
  else if (quest) {
    dungeonHandler = std::make_shared<FirstQuestDungeonHandler>();
  }
  else {
    dungeonHandler = std::make_shared<SecondQuestDungeonHandler>();
  }
  std::vector<int> overworldData;
  if (quest) {
    std::vector<int> fqData = {0,0,1,1,0,0,0,0,0,0,1,0,0,0,1,1,
			       1,1,0,0,0,1,0,0,0,1,0,0,1,0,1,0,
			       1,0,1,1,1,1,0,1,1,1,1,0,1,1,0,1,
			       1,1,0,1,0,0,0,0,0,0,0,0,1,0,0,1,
			       1,1,1,0,0,1,1,0,1,1,1,1,0,0,1,1,
			       1,0,0,0,0,0,0,0,0,1,1,1,0,0,1,0,
			       0,1,0,0,0,1,0,1,1,1,0,1,0,0,0,0,
			       1,0,1,0,0,0,1,0,1,1,0,0,0,0,0,0};
    overworldData = fqData;
  }
  else {
    std::vector<int> sqData = {0,1,0,1,0,0,0,0,0,0,1,1,0,0,1,1,
			       0,1,1,0,0,1,0,1,0,1,0,1,0,1,0,0,
			       1,0,1,0,1,1,0,1,0,1,1,0,1,1,0,1,
			       1,1,1,1,0,0,0,1,0,0,0,0,1,0,0,1,
			       0,1,1,0,0,1,1,0,1,1,0,1,0,0,1,1,
			       0,1,0,0,0,0,0,1,0,0,1,0,1,0,1,0,
			       0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,
			       0,0,0,0,0,1,0,0,1,0,0,1,0,0,0,0};
    overworldData = sqData;
  }  
  for (int x = 0; x < 16; ++x) {
    for (int y = 0; y < 8; ++y) {
      if (overworldData[x + 16*(7-y)]) {
	overworldSecrets[std::make_pair(x,y)] = Secrets::EXPLORED_CAVE;
      }
      else {
	overworldSecrets[std::make_pair(x,y)] = Secrets::UNEXPLORED;
      }
    }
  }
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
  default:
    return Secrets::UNKNOWN_DUNGEON;
  }
}

Dungeon::DungeonType ZeldaInformationHandler::AsDungeonType(Secrets type) {
  switch(type) {
  case Secrets::DUNGEON_1:
    return Dungeon::DungeonType::DUNGEON_1;
  case Secrets::DUNGEON_2:
    return Dungeon::DungeonType::DUNGEON_2;
  case Secrets::DUNGEON_3:
    return Dungeon::DungeonType::DUNGEON_3;
  case Secrets::DUNGEON_4:
    return Dungeon::DungeonType::DUNGEON_4;
  case Secrets::DUNGEON_5:
    return Dungeon::DungeonType::DUNGEON_5;
  case Secrets::DUNGEON_6:
    return Dungeon::DungeonType::DUNGEON_6;
  case Secrets::DUNGEON_7:
    return Dungeon::DungeonType::DUNGEON_7;
  case Secrets::DUNGEON_8:
    return Dungeon::DungeonType::DUNGEON_8;
  case Secrets::DUNGEON_9:
    return Dungeon::DungeonType::DUNGEON_9;
  default:
    return Dungeon::DungeonType::UNKNOWN_DUNGEON;    
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
  if (overworldTextures.find(type) == overworldTextures.end()) {
    switch (type) {
    case Secrets::UNEXPLORED:
      overworldTextures[type] = OpenGLUtility::Load2DTexture("Images/Selectors/Transparent.png", GL_RGBA);
      break;      
    case Secrets::UNKNOWN_CAVE:
      overworldTextures[type] = OpenGLUtility::Load2DTexture("Images/Selectors/OverworldExplored.png", GL_RGBA);
      break;      
    case Secrets::EXPLORED_CAVE:
      overworldTextures[type] = OpenGLUtility::Load2DTexture("Images/Selectors/OverworldExplored.png", GL_RGBA);
      break;      
    case Secrets::UNKNOWN_DUNGEON:
      overworldTextures[type] = OpenGLUtility::Load2DTexture("Images/Selectors/OverworldUnknownDungeon.png", GL_RGBA);
      break;      
    case Secrets::DUNGEON_1:
      overworldTextures[type] = OpenGLUtility::Load2DTexture("Images/Selectors/OverworldDungeon1.png", GL_RGBA);
      break;      
    case Secrets::DUNGEON_2:
      overworldTextures[type] = OpenGLUtility::Load2DTexture("Images/Selectors/OverworldDungeon2.png", GL_RGBA);
      break;      
    case Secrets::DUNGEON_3:
      overworldTextures[type] = OpenGLUtility::Load2DTexture("Images/Selectors/OverworldDungeon3.png", GL_RGBA);
      break;      
    case Secrets::DUNGEON_4:
      overworldTextures[type] = OpenGLUtility::Load2DTexture("Images/Selectors/OverworldDungeon4.png", GL_RGBA);
      break;      
    case Secrets::DUNGEON_5:
      overworldTextures[type] = OpenGLUtility::Load2DTexture("Images/Selectors/OverworldDungeon5.png", GL_RGBA);
      break;      
    case Secrets::DUNGEON_6:
      overworldTextures[type] = OpenGLUtility::Load2DTexture("Images/Selectors/OverworldDungeon6.png", GL_RGBA);
      break;      
    case Secrets::DUNGEON_7:
      overworldTextures[type] = OpenGLUtility::Load2DTexture("Images/Selectors/OverworldDungeon7.png", GL_RGBA);
      break;      
    case Secrets::DUNGEON_8:
      overworldTextures[type] = OpenGLUtility::Load2DTexture("Images/Selectors/OverworldDungeon8.png", GL_RGBA);
      break;      
    case Secrets::DUNGEON_9:
      overworldTextures[type] = OpenGLUtility::Load2DTexture("Images/Selectors/OverworldDungeon9.png", GL_RGBA);
      break;      
    case Secrets::ARROW_SHOP:
      overworldTextures[type] = OpenGLUtility::Load2DTexture("Images/Selectors/ArrowShop.png", GL_RGBA);
      break;      
    case Secrets::BAIT_SHOP:
      overworldTextures[type] = OpenGLUtility::Load2DTexture("Images/Selectors/BaitShop.png", GL_RGBA);
      break;      
    case Secrets::BOMB_SHOP:
      overworldTextures[type] = OpenGLUtility::Load2DTexture("Images/Selectors/BombShop.png", GL_RGBA);
      break;      
    case Secrets::CANDLE_SHOP:
      overworldTextures[type] = OpenGLUtility::Load2DTexture("Images/Selectors/CandleShop.png", GL_RGBA);
      break;      
    case Secrets::BLUE_RING_SHOP:
      overworldTextures[type] = OpenGLUtility::Load2DTexture("Images/Selectors/BlueRingShop.png", GL_RGBA);
      break;      
    case Secrets::ANYROAD:
      overworldTextures[type] = OpenGLUtility::Load2DTexture("Images/Selectors/Anyroad.png", GL_RGBA);
      break;      
    case Secrets::PRE_POTION_SHOP:
      overworldTextures[type] = OpenGLUtility::Load2DTexture("Images/Selectors/PotionShop.png", GL_RGBA);
      break;      
    case Secrets::POTION_SHOP:
      overworldTextures[type] = OpenGLUtility::Load2DTexture("Images/Selectors/PotionShop.png", GL_RGBA);
      break;      
    case Secrets::WOOD_SWORD:
      overworldTextures[type] = OpenGLUtility::Load2DTexture("Images/Selectors/WoodSword.png", GL_RGBA);
      break;      
    case Secrets::WHITE_SWORD:
      overworldTextures[type] = OpenGLUtility::Load2DTexture("Images/Selectors/WhiteSword.png", GL_RGBA);
      break;      
    case Secrets::MAGICAL_SWORD:
      overworldTextures[type] = OpenGLUtility::Load2DTexture("Images/Selectors/MagicalSword.png", GL_RGBA);
      break;      
    case Secrets::BONUS_CAVE:
      overworldTextures[type] = OpenGLUtility::Load2DTexture("Images/Selectors/OverworldExplored.png", GL_RGBA);
      break;      
    }
  }
  return overworldTextures[type];
}

GLuint ZeldaInformationHandler::GetTexture(Dungeon::RoomType type) {
  if (dungeonTextures.find(type) == dungeonTextures.end()) {
    switch (type) {
      case Dungeon::RoomType::UNEXPLORED:
	dungeonTextures[type] = OpenGLUtility::Load2DTexture("Images/Selectors/Transparent.png", GL_RGBA);
	break;      
      case Dungeon::RoomType::UNKNOWN_ROOM:
	dungeonTextures[type] = OpenGLUtility::Load2DTexture("Images/Selectors/DungeonRoom.png", GL_RGBA);
	break;      
      case Dungeon::RoomType::UNSEEN_ROOM:
	dungeonTextures[type] = OpenGLUtility::Load2DTexture("Images/Selectors/DungeonUnseenRoom.png", GL_RGBA);
	break;      
      case Dungeon::RoomType::GUESS_ROOM:
	dungeonTextures[type] = OpenGLUtility::Load2DTexture("Images/Selectors/DungeonUnseenRoom.png", GL_RGBA);
	break;      
      case Dungeon::RoomType::TRIFORCE_ROOM:
	dungeonTextures[type] = OpenGLUtility::Load2DTexture("Images/Selectors/DungeonTriforceRoom.png", GL_RGBA);
	break;      
      case Dungeon::RoomType::STAIRCASE_1:
	dungeonTextures[type] = OpenGLUtility::Load2DTexture("Images/Selectors/Staircase1.png", GL_RGBA);
	break;      
      case Dungeon::RoomType::STAIRCASE_2:
	dungeonTextures[type] = OpenGLUtility::Load2DTexture("Images/Selectors/Staircase2.png", GL_RGBA);
	break;      
      case Dungeon::RoomType::STAIRCASE_3:
	dungeonTextures[type] = OpenGLUtility::Load2DTexture("Images/Selectors/Staircase3.png", GL_RGBA);
	break;      
      case Dungeon::RoomType::STAIRCASE_4:
	dungeonTextures[type] = OpenGLUtility::Load2DTexture("Images/Selectors/Staircase4.png", GL_RGBA);
	break;      
      case Dungeon::RoomType::STAIRCASE_5:
	dungeonTextures[type] = OpenGLUtility::Load2DTexture("Images/Selectors/Staircase5.png", GL_RGBA);
	break;      
      case Dungeon::RoomType::STAIRCASE_6:
	dungeonTextures[type] = OpenGLUtility::Load2DTexture("Images/Selectors/Staircase6.png", GL_RGBA);
	break;      
      case Dungeon::RoomType::STAIRCASE_7:
	dungeonTextures[type] = OpenGLUtility::Load2DTexture("Images/Selectors/Staircase7.png", GL_RGBA);
	break;      
    }
  }
  return dungeonTextures[type];
}

GLuint ZeldaInformationHandler::GetTexture(Dungeon::DoorType type) {
  if (doorTextures.find(type) == doorTextures.end()) {
    switch (type) {
      case Dungeon::DoorType::UNEXPLORED:
	doorTextures[type] = OpenGLUtility::Load2DTexture("Images/Selectors/Transparent.png", GL_RGBA);
	break;      
      case Dungeon::DoorType::OPEN:
	doorTextures[type] = OpenGLUtility::Load2DTexture("Images/Selectors/DungeonOpenDoor.png", GL_RGBA);
	break;      
      case Dungeon::DoorType::SHUTTER:
	doorTextures[type] = OpenGLUtility::Load2DTexture("Images/Selectors/DungeonShutterDoor.png", GL_RGBA);
	break;      
      case Dungeon::DoorType::KEY:
	doorTextures[type] = OpenGLUtility::Load2DTexture("Images/Selectors/DungeonKeyDoor.png", GL_RGBA);
	break;      
      case Dungeon::DoorType::BOMB:
	doorTextures[type] = OpenGLUtility::Load2DTexture("Images/Selectors/DungeonBombDoor.png", GL_RGBA);
	break;      
    }
  }
  return doorTextures[type];
}

GLuint ZeldaInformationHandler::GetTexture(Dungeon::DungeonItems type) {
  if (itemTextures.find(type) == itemTextures.end()) {
    switch (type) {
      case Dungeon::DungeonItems::NONE:
	itemTextures[type] = OpenGLUtility::Load2DTexture("Images/Selectors/PreItem.png", GL_RGBA);
	break;      
      case Dungeon::DungeonItems::BOOK:
	itemTextures[type] = OpenGLUtility::Load2DTexture("Images/Selectors/Book.png", GL_RGBA);
	break;      
      case Dungeon::DungeonItems::BOW:
	itemTextures[type] = OpenGLUtility::Load2DTexture("Images/Selectors/Bow.png", GL_RGBA);
	break;      
      case Dungeon::DungeonItems::HEART_CONTAINER:
	itemTextures[type] = OpenGLUtility::Load2DTexture("Images/Selectors/HeartContainer.png", GL_RGBA);
	break;      
      case Dungeon::DungeonItems::LADDER:
	itemTextures[type] = OpenGLUtility::Load2DTexture("Images/Selectors/Ladder.png", GL_RGBA);
	break;      
      case Dungeon::DungeonItems::MAGICAL_BOOMERANG:
	itemTextures[type] = OpenGLUtility::Load2DTexture("Images/Selectors/MagicalBoomerang.png", GL_RGBA);
	break;      
      case Dungeon::DungeonItems::WOODEN_BOOMERANG:
	itemTextures[type] = OpenGLUtility::Load2DTexture("Images/Selectors/WoodenBoomerang.png", GL_RGBA);
	break;      
      case Dungeon::DungeonItems::MAGICAL_KEY:
	itemTextures[type] = OpenGLUtility::Load2DTexture("Images/Selectors/MagicalKey.png", GL_RGBA);
	break;      
      case Dungeon::DungeonItems::RAFT:
	itemTextures[type] = OpenGLUtility::Load2DTexture("Images/Selectors/Raft.png", GL_RGBA);
	break;      
      case Dungeon::DungeonItems::RECORDER:
	itemTextures[type] = OpenGLUtility::Load2DTexture("Images/Selectors/Recorder.png", GL_RGBA);
	break;      
      case Dungeon::DungeonItems::RED_CANDLE:
	itemTextures[type] = OpenGLUtility::Load2DTexture("Images/Selectors/RedCandle.png", GL_RGBA);
	break;      
      case Dungeon::DungeonItems::RED_RING:
	itemTextures[type] = OpenGLUtility::Load2DTexture("Images/Selectors/RedRing.png", GL_RGBA);
	break;      
      case Dungeon::DungeonItems::POWER_BRACELET:
	itemTextures[type] = OpenGLUtility::Load2DTexture("Images/Selectors/PowerBracelet.png", GL_RGBA);
	break;      
      case Dungeon::DungeonItems::SILVER_ARROW:
	itemTextures[type] = OpenGLUtility::Load2DTexture("Images/Selectors/SilverArrow.png", GL_RGBA);
	break;      
      case Dungeon::DungeonItems::WAND:
	itemTextures[type] = OpenGLUtility::Load2DTexture("Images/Selectors/Wand.png", GL_RGBA);
	break;      
      case Dungeon::DungeonItems::WHITE_SWORD:
	itemTextures[type] = OpenGLUtility::Load2DTexture("Images/Selectors/WhiteSwordItem.png", GL_RGBA);
	break;      
    }
  }
  return itemTextures[type];
}
