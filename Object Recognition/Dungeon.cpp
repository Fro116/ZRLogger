#include "Dungeon.h"

Dungeon::Dungeon(int x, int y) {
  overworldx = x;
  overworldy = y;
  levelNumber = DungeonType::UNKNOWN_DUNGEON;
  triforce = false;
  heart = false;
  firstItem = DungeonItems::NONE;
  secondItem = DungeonItems::NONE;
  firstitemloc = std::make_pair(-1, -1);
  seconditemloc = std::make_pair(-1, -1);
  staircaseNumber = 0;
}

void Dungeon::SetLocation(int x, int y, RoomType type) {
  bool write = true;
  RoomType prev = GetRoomType(x,y);
  if (prev == type) {
    write = false;
  }
  //prevent overriding data  
  if (type == RoomType::UNSEEN_ROOM && !(prev == RoomType::UNEXPLORED || prev == RoomType::GUESS_ROOM)) {
    write = false;
  }
  if (type != RoomType::TRIFORCE_ROOM && (prev == RoomType::STAIRCASE_1 || prev == RoomType::STAIRCASE_2 || prev == RoomType::STAIRCASE_3
					  || prev == RoomType::STAIRCASE_4 || prev == RoomType::STAIRCASE_5 || prev == RoomType::STAIRCASE_6
					  || prev == RoomType::STAIRCASE_7)) {
    write = false;
  }
  if (prev == RoomType::TRIFORCE_ROOM) {
    write = false;
  }
  if (type == RoomType::TRIFORCE_ROOM) {
    for (int x = 0; x < 8; ++x) {
      for (int y = 0; y < 8; ++y) {
	if (GetRoomType(x,y) == RoomType::TRIFORCE_ROOM) {
	  write = false;
	}
      }
    }
  }
  if (x < 0 || y < 0 || x >= 8 || y >=8) {
    write = false;
  }
  if (write) {
    //store starting location
    if (type == RoomType::UNSEEN_ROOM) {
      bool foundStart = false;
      for (auto& el : rooms) {
	if (el.second == RoomType::UNSEEN_ROOM) {
	  foundStart = true;
	}
      }
      if (!foundStart) {
	startloc = std::make_pair(x, y);
      }
    }
    rooms[std::make_pair(x, y)] = type;
  }
}

void Dungeon::SetDungeonType(DungeonType level) {
  levelNumber = level;
  // levelNumber = DungeonType::DUNGEON_9;
  // int level = 8;
  // for (int a = 0; a < 8; ++a) {
  //   for (int b = 0; b < 8; ++b) {
  //     if (dungeonShapes[level][a][b] && GetRoomType(a,b) == RoomType::UNEXPLORED) {
  // 	rooms[std::make_pair(a, b)] = RoomType::UNSEEN_ROOM;
  //     }
  //   }
  // }  
}

Dungeon::RoomType Dungeon::GetRoomType(int x, int y) {
  auto it = rooms.find(std::make_pair(x,y));
  if (it == rooms.end()) {
    return RoomType::UNEXPLORED;
  }
  else {
    return it->second;
  }
}

std::map<std::pair<int, int>, Dungeon::RoomType> Dungeon::GetRooms() {
  return rooms;
}

void Dungeon::SetDoorType(int x1, int y1, int x2, int y2, DoorType type) {
  bool set = true;
  DoorType prev = GetDoorType(x1, y1, x2, y2);
  if (set) {
    std::tuple<int, int, int, int> d1 = std::make_tuple(x1,y1,x2,y2);
    std::tuple<int, int, int, int> d2 = std::make_tuple(x2,y2,x1,y1);
    doors[d1] = type;
    doors[d2] = type;
  }
}

Dungeon::DoorType Dungeon::GetDoorType(int x1, int y1, int x2, int y2) {
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

std::pair<int, int> Dungeon::GetLocation() {
  return std::make_pair(overworldx,overworldy);
}

std::pair<int, int> Dungeon::GetStartLocation() {
  return startloc;
} 

Dungeon::DungeonType Dungeon::Number() {
  return levelNumber;
}

bool Dungeon::GetTriforce() {
  return triforce;
}

void Dungeon::SetTriforce() {
  triforce = true;
}

bool Dungeon::GetHeart() {
  return heart;
}

void Dungeon::SetHeart() {
  heart = true;
}

Dungeon::DungeonItems Dungeon::GetFirstItem() {
  return firstItem;
}

Dungeon::DungeonItems Dungeon::GetSecondItem() {
  return secondItem;
}

void Dungeon::SetItem(DungeonItems item, std::pair<int, int> location) {
  if (firstItem == DungeonItems::NONE) {
    firstItem = item;
    firstitemloc = location;
  }
  else {
    if (firstitemloc == location) {
      //prevent overwriting
    }
    else {
      secondItem = item;
      seconditemloc = location;
    }
  }
}

Dungeon::DungeonItems Dungeon::GetItem(std::pair<int, int> loc) {
  if (firstitemloc == loc) {
    return firstItem;
  }
  if (seconditemloc == loc) {
    return secondItem;
  }
  return DungeonItems::NONE;
}

void Dungeon::SetStaircase(std::pair<int, int> firstroom, std::pair<int, int> secondroom) {
  RoomType type = GetRoomType(firstroom.first, firstroom.second);
  if (type != RoomType::STAIRCASE_1 && type != RoomType::STAIRCASE_2 && type != RoomType::STAIRCASE_3 && type != RoomType::STAIRCASE_4
      && type != RoomType::STAIRCASE_5 && type != RoomType::STAIRCASE_6 && type != RoomType::STAIRCASE_7) {
    RoomType secret;
    if (staircaseNumber == 0) {
      secret = RoomType::STAIRCASE_1;
    }
    if (staircaseNumber == 1) {
      secret = RoomType::STAIRCASE_2;
    }
    if (staircaseNumber == 2) {
      secret = RoomType::STAIRCASE_3;
    }
    if (staircaseNumber == 3) {
      secret = RoomType::STAIRCASE_4;
    }
    if (staircaseNumber == 4) {
      secret = RoomType::STAIRCASE_5;
    }
    if (staircaseNumber == 5) {
      secret = RoomType::STAIRCASE_6;
    }
    if (staircaseNumber == 6) {
      secret = RoomType::STAIRCASE_7;
    }
    rooms[firstroom] = secret;
    rooms[secondroom] = secret;    
    staircaseNumber++;
  }
}

