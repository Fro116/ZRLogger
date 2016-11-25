#include "DungeonHandler.h"

DungeonHandler::DungeonHandler() {

}

std::vector<std::vector<bool>> DungeonHandler::FormatShape(int shape[]) {
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

Dungeon::DungeonType DungeonHandler::AsDungeonType(int level) {
  if (level == 0) {
    return Dungeon::DungeonType::DUNGEON_1;
  }
  if (level == 1) {
    return Dungeon::DungeonType::DUNGEON_2;
  }
  if (level == 2) {
    return Dungeon::DungeonType::DUNGEON_3;
  }
  if (level == 3) {
    return Dungeon::DungeonType::DUNGEON_4;
  }
  if (level == 4) {
    return Dungeon::DungeonType::DUNGEON_5;
  }
  if (level == 5) {
    return Dungeon::DungeonType::DUNGEON_6;
  }
  if (level == 6) {
    return Dungeon::DungeonType::DUNGEON_7;
  }
  if (level == 7) {
    return Dungeon::DungeonType::DUNGEON_8;
  }
  return Dungeon::DungeonType::UNKNOWN_DUNGEON;
}

int DungeonHandler::AsNumber(Dungeon::DungeonType level) {
  switch (level) {
  case Dungeon::DungeonType::DUNGEON_1:
    return 0;
  case Dungeon::DungeonType::DUNGEON_2:
    return 1;
  case Dungeon::DungeonType::DUNGEON_3:
    return 2;
  case Dungeon::DungeonType::DUNGEON_4:
    return 3;
  case Dungeon::DungeonType::DUNGEON_5:
    return 4;
  case Dungeon::DungeonType::DUNGEON_6:
    return 5;
  case Dungeon::DungeonType::DUNGEON_7:
    return 6;
  case Dungeon::DungeonType::DUNGEON_8:
    return 7;
  case Dungeon::DungeonType::DUNGEON_9:
    return 8;
  case Dungeon::DungeonType::UNKNOWN_DUNGEON:
    return -1;
  }
}
