#include "FirstQuestDungeonHandler.h"

FirstQuestDungeonHandler::FirstQuestDungeonHandler() : DungeonHandler(), dungeonShapes(), pastGuess() {
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

void FirstQuestDungeonHandler::PredictLevel(Dungeon& dungeon) {
  //Check which dungeon you are in
  std::vector<int> possible;
  for (int level = 0; level < 8; ++level) {
    int right = 0;
    int wrong = 0;
    for (auto& el : dungeon.GetRooms()) {
      if (el.second != Dungeon::RoomType::GUESS_ROOM && el.second != Dungeon::RoomType::UNEXPLORED) {
	if (dungeonShapes[level][el.first.first][el.first.second]) {
	  right++;
	}
	else {
	  wrong++;
	}
      }
    }
    if (wrong == 0) {
      possible.push_back(level);
    }
  }
  if (possible.size() == 1) {
    dungeon.SetDungeonType(AsDungeonType(possible[0]));
  }
  //Because some dungeons look alike, make an educated guess
  if (possible.size() == 2) {
    int a = possible[0];
    int b = possible[1];
    if ((a == 0 && b == 2) || (a == 2 && b == 0)) {
      dungeon.SetDungeonType(Dungeon::DungeonType::DUNGEON_3);
    }
    int seen = 0;
    for (auto& el : dungeon.GetRooms()) {
      if (el.second != Dungeon::RoomType::GUESS_ROOM && el.second != Dungeon::RoomType::UNEXPLORED && el.second != Dungeon::RoomType::UNSEEN_ROOM) {
	seen++;
      }
    }
    if (seen >=2) {
      if ((a == 6 && b == 3) || (a == 3 && b == 6)) {
	dungeon.SetDungeonType(Dungeon::DungeonType::DUNGEON_4);
      }
      if ((a == 6 && b == 5) || (a == 5 && b == 6)) {
	dungeon.SetDungeonType(Dungeon::DungeonType::DUNGEON_6);
      }
    }
  }
  Dungeon::DungeonType pastnum = Dungeon::DungeonType::UNKNOWN_DUNGEON;
  if (pastGuess.find(dungeon.GetLocation()) != pastGuess.end()) {
    pastnum = pastGuess[dungeon.GetLocation()];
  }
  int level = AsNumber(dungeon.Number());
  if(level != -1 && pastnum != dungeon.Number()) {
    for (int a = 0; a < 8; ++a) {
      for (int b = 0; b < 8; ++b) {
	if (dungeonShapes[level][a][b]) {
	}
	if (dungeonShapes[level][a][b] && dungeon.GetRoomType(a,b) == Dungeon::RoomType::UNEXPLORED) {
	  dungeon.SetLocation(a, b, Dungeon::RoomType::GUESS_ROOM);
	}
	if (!dungeonShapes[level][a][b]) {
	  dungeon.SetLocation(a, b, Dungeon::RoomType::UNEXPLORED);	    
	}
      }
    }
  }
  pastGuess[dungeon.GetLocation()] = dungeon.Number();
}
