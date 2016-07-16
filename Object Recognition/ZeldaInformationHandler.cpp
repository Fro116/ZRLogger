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
}

void ZeldaInformationHandler::InitTextures() {
  OpenGLUtility::Load2DTexture("Images/Selectors/OverworldDungeon3.png", GL_RGBA);
  overworldTextures[Secrets::UNKNOWN_CAVE] = OpenGLUtility::Load2DTexture("Images/Selectors/OverworldExplored.png", GL_RGBA);
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

  dungeonTextures[RoomType::UNEXPLORED] = OpenGLUtility::Load2DTexture("Images/Selectors/Transparent.png", GL_RGBA);
  dungeonTextures[RoomType::UNKNOWN_ROOM] = OpenGLUtility::Load2DTexture("Images/Selectors/DungeonRoom.png", GL_RGBA);
  dungeonTextures[RoomType::UNSEEN_ROOM] = OpenGLUtility::Load2DTexture("Images/Selectors/DungeonUnseenRoom.png", GL_RGBA);
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
  dungeonx = x;
  dungeony = y;
}


std::pair<int, int> ZeldaInformationHandler::GetDungeonLocation() {
  std::lock_guard<std::recursive_mutex> guard(dataMutex);
  return std::make_pair(dungeonx, dungeony);
}



ZeldaInformationHandler::Secrets ZeldaInformationHandler::GetSecret(int x, int y) {
  std::lock_guard<std::recursive_mutex> guard(dataMutex);
  return overworldSecrets[std::make_pair(x,y)];
}

void ZeldaInformationHandler::SetSecret(int x, int y, Secrets secret) {
  std::lock_guard<std::recursive_mutex> guard(dataMutex);
  overworldSecrets[std::make_pair(x,y)] = secret;
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
    if (possible.size() == 1) {
      int level = possible[0];
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
      for (int a = 0; a < 8; ++a) {
	for (int b = 0; b < 8; ++b) {
	  if (dungeonShapes[level][a][b] && GetRoomType(a,b) == RoomType::UNEXPLORED) {
	    rooms[std::make_pair(a, b)] = RoomType::UNSEEN_ROOM;
	  }
	}
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

std::pair<int, int> ZeldaInformationHandler::Dungeon::GetLocation() {
  return std::make_pair(overworldx,overworldy);
}

ZeldaInformationHandler::Secrets ZeldaInformationHandler::Dungeon::Number() {
  return levelNumber;
}

GLuint ZeldaInformationHandler::GetTexture(Secrets type) {
  return overworldTextures[type];
}

GLuint ZeldaInformationHandler::GetTexture(RoomType type) {
  return dungeonTextures[type];
}
