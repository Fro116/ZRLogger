#include "ZeldaInformationHandler.h"

std::recursive_mutex ZeldaInformationHandler::dataMutex;

int ZeldaInformationHandler::mapx = -1;
int ZeldaInformationHandler::mapy = -1;
int ZeldaInformationHandler::dungeonx = -1;
int ZeldaInformationHandler::dungeony = -1;

std::map<std::pair<int,int>, ZeldaInformationHandler::Secrets> ZeldaInformationHandler::overworldSecrets;
bool ZeldaInformationHandler::isRunning = true;
bool ZeldaInformationHandler::isInDungeon = false;
std::vector<ZeldaInformationHandler::Dungeon> ZeldaInformationHandler::dungeons;
std::vector<std::vector<std::vector<bool>>> ZeldaInformationHandler::dungeonShapes;

std::map<ZeldaInformationHandler::Secrets, GLuint> ZeldaInformationHandler::overworldTextures;
std::map<ZeldaInformationHandler::RoomType, GLuint> ZeldaInformationHandler::dungeonTextures;
std::map<ZeldaInformationHandler::DoorType, GLuint> ZeldaInformationHandler::doorTextures;

bool ZeldaInformationHandler::zeldaScreenFound;

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
  overworldTextures[Secrets::PRE_POTION_SHOP] = OpenGLUtility::Load2DTexture("Images/Selectors/PotionShop.png", GL_RGBA);
  overworldTextures[Secrets::POTION_SHOP] = OpenGLUtility::Load2DTexture("Images/Selectors/PotionShop.png", GL_RGBA);        
  overworldTextures[Secrets::WHITE_SWORD] = OpenGLUtility::Load2DTexture("Images/Selectors/WhiteSword.png", GL_RGBA);
  overworldTextures[Secrets::MAGICAL_SWORD] = OpenGLUtility::Load2DTexture("Images/Selectors/MagicalSword.png", GL_RGBA);
  overworldTextures[Secrets::BONUS_CAVE] = OpenGLUtility::Load2DTexture("Images/Selectors/OverworldExplored.png", GL_RGBA);  
      
  dungeonTextures[RoomType::UNEXPLORED] = OpenGLUtility::Load2DTexture("Images/Selectors/Transparent.png", GL_RGBA);
  dungeonTextures[RoomType::UNKNOWN_ROOM] = OpenGLUtility::Load2DTexture("Images/Selectors/DungeonRoom.png", GL_RGBA);
  dungeonTextures[RoomType::UNSEEN_ROOM] = OpenGLUtility::Load2DTexture("Images/Selectors/DungeonUnseenRoom.png", GL_RGBA);

  doorTextures[DoorType::UNEXPLORED] = OpenGLUtility::Load2DTexture("Images/Selectors/Transparent.png", GL_RGBA);
  doorTextures[DoorType::OPEN] = OpenGLUtility::Load2DTexture("Images/Selectors/DungeonOpenDoor.png", GL_RGBA);
  doorTextures[DoorType::SHUTTER] = OpenGLUtility::Load2DTexture("Images/Selectors/DungeonShutterDoor.png", GL_RGBA);
  doorTextures[DoorType::KEY] = OpenGLUtility::Load2DTexture("Images/Selectors/DungeonKeyDoor.png", GL_RGBA);
  doorTextures[DoorType::BOMB] = OpenGLUtility::Load2DTexture("Images/Selectors/DungeonBombDoor.png", GL_RGBA);  
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
  mapx = x;
  mapy = y;
}


std::pair<int, int> ZeldaInformationHandler::GetMapLocation() {
  std::lock_guard<std::recursive_mutex> guard(dataMutex);
  return std::make_pair(mapx, mapy);
}

void ZeldaInformationHandler::SetDungeonLocation(int x, int y, RoomType type) {
  std::lock_guard<std::recursive_mutex> guard(dataMutex);
  SetIsInDungeon(true);
  std::pair<int, int> loc = GetMapLocation();
  bool found = false;
  Secrets number;
  for (auto& el : dungeons) {
    if (el.GetLocation() == loc) {
      el.SetLocation(x, y, type);
      found = true;
      number = el.Number();
    }
  }
  if (!found) {
    Dungeon dungeon(loc.first,loc.second);
    dungeon.SetLocation(x, y, type);
    dungeons.push_back(dungeon);
    number = dungeon.Number();
  }
  SetSecret(loc.first, loc.second, number);
  if (type != RoomType::UNSEEN_ROOM) {
    dungeonx = x;
    dungeony = y;
  }
}


std::pair<int, int> ZeldaInformationHandler::GetDungeonLocation() {
  std::lock_guard<std::recursive_mutex> guard(dataMutex);
  return std::make_pair(dungeonx, dungeony);
}

bool ZeldaInformationHandler::GetTriforce(int level) {
  std::lock_guard<std::recursive_mutex> guard(dataMutex);
  Secrets levelNumber;
  if (level == 0) {
    levelNumber = Secrets::DUNGEON_1;
  }
  if (level == 1) {
    levelNumber = Secrets::DUNGEON_2;
  }
  if (level == 2) {
    levelNumber = Secrets::DUNGEON_3;
  }
  if (level == 3) {
    levelNumber = Secrets::DUNGEON_4;
  }
  if (level == 4) {
    levelNumber = Secrets::DUNGEON_5;
  }
  if (level == 5) {
    levelNumber = Secrets::DUNGEON_6;
  }
  if (level == 6) {
    levelNumber = Secrets::DUNGEON_7;
  }
  if (level == 7) {
    levelNumber = Secrets::DUNGEON_8;
  }
  for (auto& el : dungeons) {
    if (el.Number() == levelNumber) {
      return el.GetTriforce();
    }
  }  
  return false;
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
	|| prev == Secrets::BONUS_CAVE) {
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

void ZeldaInformationHandler::SetIsInDungeon(bool inDungeon) {
  std::lock_guard<std::recursive_mutex> guard(dataMutex);
  isInDungeon = inDungeon;
}

ZeldaInformationHandler::RoomType ZeldaInformationHandler::GetDungeonRoomType(int x, int y) {
  std::lock_guard<std::recursive_mutex> guard(dataMutex);
  std::pair<int, int> loc = GetMapLocation();
  for (auto& el : dungeons) {
    if (el.GetLocation() == loc) {
      return el.GetRoomType(x, y);
    }
  }
  return RoomType::UNEXPLORED;
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

ZeldaInformationHandler::DoorType ZeldaInformationHandler::GetDungeonDoor(int x1, int y1, int x2, int y2) {
  std::lock_guard<std::recursive_mutex> guard(dataMutex);
  std::pair<int, int> loc = GetMapLocation();
  for (auto& el : dungeons) {
    if (el.GetLocation() == loc) {
      return el.GetDoorType(x1, y1, x2, y2);
    }
  }
  return DoorType::UNEXPLORED;  
}

void ZeldaInformationHandler::SetDungeonDoor(int x1, int y1, int x2, int y2, DoorType type) {
  std::lock_guard<std::recursive_mutex> guard(dataMutex);
  std::pair<int, int> loc = GetMapLocation();
  for (auto& el : dungeons) {
    if (el.GetLocation() == loc) {
      el.SetDoorType(x1, y1, x2, y2, type);
    }
  }
}

ZeldaInformationHandler::Dungeon::Dungeon(int x, int y) {
  overworldx = x;
  overworldy = y;
  levelNumber = Secrets::UNKNOWN_DUNGEON;
}

void ZeldaInformationHandler::Dungeon::SetLocation(int x, int y, RoomType type) {
  bool write = true;
  RoomType prev = GetRoomType(x,y);
  if (prev == type) {
    write = false;
  }
  if (type == RoomType::UNSEEN_ROOM && prev != RoomType::UNEXPLORED) {
    //prevent overriding data
    write = false;
  }
  if (x < 0 || y < 0 || x >= 8 || y >=8) {
    write = false;
  }
  if (write) {
    rooms[std::make_pair(x, y)] = type;
    if (levelNumber != Secrets::DUNGEON_9) {
      //Check which dungeon you are in
      std::vector<int> possible;
      for (int level = 0; level < 8; ++level) {
	int right = 0;
	int wrong = 0;
	for (auto& el : rooms) {
	  if (dungeonShapes[level][el.first.first][el.first.second]) {
	    right++;
	  }
	  else {
	    wrong++;
	  }
	}
	if (wrong == 0) {
	  possible.push_back(level);
	}
      }
      int level = -1;
      if (possible.size() == 1) {
	level = possible[0];
	if (level == 0) {
	  levelNumber = Secrets::DUNGEON_1;
	}
	if (level == 1) {
	  levelNumber = Secrets::DUNGEON_2;
	}
	if (level == 2) {
	  levelNumber = Secrets::DUNGEON_3;
	}
	if (level == 3) {
	  levelNumber = Secrets::DUNGEON_4;
	}
	if (level == 4) {
	  levelNumber = Secrets::DUNGEON_5;
	}
	if (level == 5) {
	  levelNumber = Secrets::DUNGEON_6;
	}
	if (level == 6) {
	  levelNumber = Secrets::DUNGEON_7;
	}
	if (level == 7) {
	  levelNumber = Secrets::DUNGEON_8;
	}
      }
      //Because some dungeons look alike, make an educated guess
      if (possible.size() == 2) {
	int a = possible[0];
	int b = possible[1];
	if ((a == 0 && b == 2) || (a == 2 && b == 0)) {
	  level = 2;
	  levelNumber = Secrets::DUNGEON_3;
	}
	if ((a == 6 && b == 3) || (a == 3 && b == 6)) {
	  level = 3;
	  levelNumber = Secrets::DUNGEON_4;
	}
	if ((a == 6 && b == 5) || (a == 5 && b == 6)) {
	  level = 5;
	  levelNumber = Secrets::DUNGEON_6;
	}
      }
      if(level != -1) {
	for (int a = 0; a < 8; ++a) {
	  for (int b = 0; b < 8; ++b) {
	    if (dungeonShapes[level][a][b] && GetRoomType(a,b) == RoomType::UNEXPLORED) {
	      rooms[std::make_pair(a, b)] = RoomType::UNSEEN_ROOM;
	    }
	    if (!dungeonShapes[level][a][b]) {
	      rooms[std::make_pair(a, b)] = RoomType::UNEXPLORED;
	    }
	  }
	}
      }
    }
  }
}

void ZeldaInformationHandler::Dungeon::SetLevelNine() {
  levelNumber = Secrets::DUNGEON_9;
  int level = 8;
  for (int a = 0; a < 8; ++a) {
    for (int b = 0; b < 8; ++b) {
      if (dungeonShapes[level][a][b] && GetRoomType(a,b) == RoomType::UNEXPLORED) {
	rooms[std::make_pair(a, b)] = RoomType::UNSEEN_ROOM;
      }
    }
  }  
}

ZeldaInformationHandler::RoomType ZeldaInformationHandler::Dungeon::GetRoomType(int x, int y) {
  auto it = rooms.find(std::make_pair(x,y));
  if (it == rooms.end()) {
    return RoomType::UNEXPLORED;
  }
  else {
    return it->second;
  }
}

void ZeldaInformationHandler::Dungeon::SetDoorType(int x1, int y1, int x2, int y2, DoorType type) {
  std::tuple<int, int, int, int> d1 = std::make_tuple(x1,y1,x2,y2);
  std::tuple<int, int, int, int> d2 = std::make_tuple(x2,y2,x1,y1);
  doors[d1] = type;
  doors[d2] = type;  
}

ZeldaInformationHandler::DoorType ZeldaInformationHandler::Dungeon::GetDoorType(int x1, int y1, int x2, int y2) {
  std::tuple<int, int, int, int> d1 = std::make_tuple(x1,y1,x2,y2);
  std::tuple<int, int, int, int> d2 = std::make_tuple(x2,y2,x1,y1);
  if (doors.find(d1) != doors.end()) {
    return doors[d1];
  }
  if (doors.find(d2) != doors.end()) {
    return doors[d2];
  }
  return DoorType::UNEXPLORED;
}

std::pair<int, int> ZeldaInformationHandler::Dungeon::GetLocation() {
  return std::make_pair(overworldx,overworldy);
}

ZeldaInformationHandler::Secrets ZeldaInformationHandler::Dungeon::Number() {
  return levelNumber;
}

bool ZeldaInformationHandler::Dungeon::GetTriforce() {
  return triforce;
}

void ZeldaInformationHandler::Dungeon::SetTriforce() {
  triforce = true;
}

GLuint ZeldaInformationHandler::GetTexture(Secrets type) {
  return overworldTextures[type];
}

GLuint ZeldaInformationHandler::GetTexture(RoomType type) {
  return dungeonTextures[type];
}

GLuint ZeldaInformationHandler::GetTexture(DoorType type) {
  return doorTextures[type];
}
