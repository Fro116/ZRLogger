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
    {
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
    }
    rooms[std::make_pair(x, y)] = type;
    // if (levelNumber != DungeonType::DUNGEON_9) {
    //   //Check which dungeon you are in
    //   std::vector<int> possible;
    //   for (int level = 0; level < 8; ++level) {
    // 	int right = 0;
    // 	int wrong = 0;
    // 	for (auto& el : rooms) {
    // 	  if (el.second != RoomType::GUESS_ROOM && el.second != RoomType::UNEXPLORED) {
    // 	    if (dungeonShapes[level][el.first.first][el.first.second]) {
    // 	      right++;
    // 	    }
    // 	    else {
    // 	      wrong++;
    // 	    }
    // 	  }
    // 	}
    // 	if (wrong == 0) {
    // 	  possible.push_back(level);
    // 	}
    //   }
    //   int level = -1;
    //   if (possible.size() == 1) {
    // 	level = possible[0];
    // 	if (level == 0) {
    // 	  levelNumber = DungeonType::DUNGEON_1;
    // 	}
    // 	if (level == 1) {
    // 	  levelNumber = DungeonType::DUNGEON_2;
    // 	}
    // 	if (level == 2) {
    // 	  levelNumber = DungeonType::DUNGEON_3;
    // 	}
    // 	if (level == 3) {
    // 	  levelNumber = DungeonType::DUNGEON_4;
    // 	}
    // 	if (level == 4) {
    // 	  levelNumber = DungeonType::DUNGEON_5;
    // 	}
    // 	if (level == 5) {
    // 	  levelNumber = DungeonType::DUNGEON_6;
    // 	}
    // 	if (level == 6) {
    // 	  levelNumber = DungeonType::DUNGEON_7;
    // 	}
    // 	if (level == 7) {
    // 	  levelNumber = DungeonType::DUNGEON_8;
    // 	}
    //   }
    //   //Because some dungeons look alike, make an educated guess
    //   if (possible.size() == 2) {
    // 	int a = possible[0];
    // 	int b = possible[1];
    // 	if ((a == 0 && b == 2) || (a == 2 && b == 0)) {
    // 	  level = 2;
    // 	  levelNumber = DungeonType::DUNGEON_3;
    // 	}
    // 	int seen = 0;
    // 	for (auto& el : rooms) {
    // 	  if (el.second != RoomType::GUESS_ROOM && el.second != RoomType::UNEXPLORED && el.second != RoomType::UNSEEN_ROOM) {
    // 	    seen++;
    // 	  }
    // 	}
    // 	if (seen >=2) {
    // 	  if ((a == 6 && b == 3) || (a == 3 && b == 6)) {
    // 	    level = 3;
    // 	    levelNumber = DungeonType::DUNGEON_4;
    // 	  }
    // 	  if ((a == 6 && b == 5) || (a == 5 && b == 6)) {
    // 	    level = 5;
    // 	    levelNumber = DungeonType::DUNGEON_6;
    // 	  }
    // 	}
    //   }
    //   if(level != -1) {
    // 	for (int a = 0; a < 8; ++a) {
    // 	  for (int b = 0; b < 8; ++b) {
    // 	    if (dungeonShapes[level][a][b] && GetRoomType(a,b) == RoomType::UNEXPLORED) {
    // 	      rooms[std::make_pair(a, b)] = RoomType::GUESS_ROOM;
    // 	    }
    // 	    if (!dungeonShapes[level][a][b]) {
    // 	      rooms[std::make_pair(a, b)] = RoomType::UNEXPLORED;
    // 	    }
    // 	  }
    // 	}
    //   }
    // }
  }
}

void Dungeon::SetLevelNine() {
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

