#include "ZeldaImageProcessor.h"
#include "ZeldaInformationHandler.h"
#include "Dungeon.h"

#include <unordered_set>
#include <stack>

ZeldaImageProcessor::ZeldaImageProcessor() {
  zeldaScreen = ImageHandler::LoadPNG("Images/Registration/RegistrationScreen.png").ConvertToBlackAndWhite();  
  while (!FindZeldaScreen() && ZeldaInformationHandler::GetIsRunning()); //Find zelda screen
  ZeldaInformationHandler::SetZeldaSceenFound(true);
  LoadTextures();
}

void ZeldaImageProcessor::UpdateData() {
  ImageHandler screen = GetScreen();
  if (!INITIALIZED_MINIMAP_GRAY) {
    InitMinimap(screen);
  }
  if (INITIALIZED_MINIMAP_GRAY && !INITIALIZED_TUNIC_GREEN) {
    InitTunic(screen);
  }
  if (INITIALIZED_CURRENT_TUNIC) {
    //first check to see if you are on the start screen or not by aligning to the first dash in "-LIFE-"
    ImageHandler life = screen.Crop(REFERENCE_HEART_RED_XCOOR*SCALE_X, REFERENCE_HEART_RED_YCOOR*SCALE_Y, REFERENCE_HEART_RED_WIDTH*SCALE_X, REFERENCE_HEART_RED_HEIGHT*SCALE_Y);
    std::tuple<int, int, int> heartrgb = life.MostCommonRGB();    
    if (heartrgb == std::make_tuple(HEART_RED_R, HEART_RED_G, HEART_RED_B)) {
      ImageHandler minimap = screen.Crop(REFERENCE_OVERWORLD_MINIMAP_XCOOR*SCALE_X, REFERENCE_OVERWORLD_MINIMAP_YCOOR*SCALE_Y, REFERENCE_OVERWORLD_MINIMAP_WIDTH*SCALE_X, REFERENCE_OVERWORLD_MINIMAP_HEIGHT*SCALE_Y);
      if (minimap.MostCommonRGB() == std::make_tuple(MINIMAP_GRAY_R, MINIMAP_GRAY_G, MINIMAP_GRAY_B)) {
	//check for overworld location
	std::tuple<bool, int, int> position = FindLinkOverworld(minimap);
	if (std::get<0>(position)) {
	  int mapx = std::get<1>(position);
	  int mapy = std::get<2>(position);
	  ZeldaInformationHandler::SetMapLocation(mapx, mapy);
	  dungeondoortransition = false;
	  CheckSecretCave(screen, mapx, mapy);	  
    CheckWhiteSwordCaveItem(screen, mapx, mapy);
	}
	else {
	  CheckOverworldRing(minimap);
	}
      }
      else {
	//check for dungeon location
	if (minimap.PixelsWithRGB(MINIMAP_GRAY_R, MINIMAP_GRAY_G, MINIMAP_GRAY_B).size() == 0) {
	  std::tuple<bool, int, int> position = FindLinkDungeon(minimap);
	  if (std::get<0>(position)) {
	    int lmapx = std::get<1>(position);
	    int lmapy = std::get<2>(position);	    
	    CheckCompass(minimap, lmapx, lmapy);
	    RecordDungeonNumber(screen);
	    RecordDoors(screen, lmapx, lmapy);
	    ZeldaInformationHandler::SetDungeonLocation(lmapx, lmapy, Dungeon::RoomType::UNKNOWN_ROOM);
	    RecordDungeonMinimap(minimap);	    
	  }
	  else {
	    RecordStaircase(screen);
	    RecordDungeonItems(screen);
	    CheckDungeonRing(minimap);	    
	  }
	  CheckTriforce(screen);	  
	}
      }
      ZeldaInformationHandler::SetHearts(Hearts(screen, true));
    }
  }
}

ImageHandler ZeldaImageProcessor::GetScreen() {
  ImageHandler fullscreen = ImageHandler::Screenshot();
  return fullscreen.Crop(CAPTURED_REGISTRATION_SCREEN_XCOOR, CAPTURED_REGISTRATION_SCREEN_YCOOR, CAPTURED_REGISTRATION_SCREEN_WIDTH, CAPTURED_REGISTRATION_SCREEN_HEIGHT);
}

bool ZeldaImageProcessor::FindZeldaScreen() {
  {
    //First index reference image
    ImageHandler reference = ImageHandler::LoadPNG("Images/Registration/RegistrationScreen.png");
    REFERENCE_SCREEN_WIDTH = reference.Width();
    REFERENCE_SCREEN_HEIGHT = reference.Height();
    std::vector<std::pair<int,int>> coordinates = reference.PixelsWithRGB(WHITE_R, WHITE_G, WHITE_B);    
    if (coordinates.empty()) {
      return false;
    }
    else {
      auto blobs = DetectBlobs(coordinates, reference.Width(), reference.Height());      
      ImageHandler zeldaC = ImageHandler::LoadPNG("Images/Registration/RegistrationScreenC.png");
      ImageHandler zeldaG = ImageHandler::LoadPNG("Images/Registration/RegistrationScreenG.png");
      for (size_t i = 0; i < blobs.size(); ++i) {
	std::vector<int> box = blobs[i];
	ImageHandler selection = reference.Crop(box[0], box[1], box[2], box[3]).ConvertToBlackAndWhite();
	double similarityC = std::min(zeldaC.Similarity(selection), selection.Similarity(zeldaC));
	double similarityG = std::min(zeldaG.Similarity(selection), selection.Similarity(zeldaG));
	if (similarityC == 1) {
	  REFERENCE_REGISTRATION_SCREEN_C_XCOOR = box[0];
	  REFERENCE_REGISTRATION_SCREEN_C_YCOOR = box[1];
	}
	if (similarityG == 1) {
	  REFERENCE_REGISTRATION_SCREEN_G_XCOOR = box[0];
	  REFERENCE_REGISTRATION_SCREEN_G_YCOOR = box[1];
	}
      }
    }
  }
  {
    //Then index screen
    ImageHandler screen = ImageHandler::Screenshot();
    screen = screen.ConvertToBlackAndWhite();
    std::vector<std::pair<int, int>> coordinates = screen.PixelsWithRGB(WHITE_R, WHITE_G, WHITE_B);
    if (coordinates.empty()) {
      return false;
    }
    else {
      auto blobs = DetectBlobs(coordinates, screen.Width(), screen.Height());
      ImageHandler zeldaC = ImageHandler::LoadPNG("Images/Registration/RegistrationScreenC.png");
      ImageHandler zeldaG = ImageHandler::LoadPNG("Images/Registration/RegistrationScreenG.png");
      double maxSimC = 0;
      size_t maxSimIndexC = 0;
      double maxSimG = 0;
      size_t maxSimIndexG = 0;
      for (size_t i = 0; i < blobs.size(); ++i) {
	std::vector<int> box = blobs[i];
	ImageHandler selection = screen.Crop(box[0], box[1], box[2], box[3]).ConvertToBlackAndWhite();
	double similarityC = 0;
	double similarityG = 0;
	if (!(selection.Width() < zeldaC.Width() || selection.Width() < zeldaG.Width() || selection.Height() < zeldaC.Height() || selection.Height() < zeldaC.Height())) {
	  //if selection is big enough
	  similarityC = selection.Similarity(zeldaC);
	  similarityG = selection.Similarity(zeldaG);
	}
	if (similarityC > maxSimC) {
	  maxSimC = similarityC;
	  maxSimIndexC = i;
	}
	if (similarityG > maxSimG) {
	  maxSimG = similarityG;
	  maxSimIndexG = i;
	}
      }
      if (maxSimC > CAPTURED_REGISTRATION_SCREEN_SIMILARITY_THRESHOLD && maxSimG > CAPTURED_REGISTRATION_SCREEN_SIMILARITY_THRESHOLD) {
	std::vector<int> boxF = blobs[maxSimIndexC];
	double fx = boxF[0];
	double fy = boxF[1];
	std::vector<int> boxG = blobs[maxSimIndexG];
	double gx = boxG[0];
	double gy = boxG[1];
	SCALE_X = (gx-fx) / static_cast<double>(REFERENCE_REGISTRATION_SCREEN_G_XCOOR - REFERENCE_REGISTRATION_SCREEN_C_XCOOR);
	SCALE_Y = (gy-fy) / static_cast<double>(REFERENCE_REGISTRATION_SCREEN_G_YCOOR - REFERENCE_REGISTRATION_SCREEN_C_YCOOR);
	CAPTURED_REGISTRATION_SCREEN_WIDTH = SCALE_X * REFERENCE_SCREEN_WIDTH;
	CAPTURED_REGISTRATION_SCREEN_HEIGHT = SCALE_Y * REFERENCE_SCREEN_HEIGHT;
	CAPTURED_REGISTRATION_SCREEN_XCOOR = gx - SCALE_X * REFERENCE_REGISTRATION_SCREEN_G_XCOOR;
	CAPTURED_REGISTRATION_SCREEN_YCOOR = gy - SCALE_Y * REFERENCE_REGISTRATION_SCREEN_G_YCOOR;
	ImageHandler screen = GetScreen().ConvertToBlackAndWhite();
	double sim = zeldaScreen.Similarity(screen);
	if (sim < CAPTURED_REGISTRATION_SCREEN_SIMILARITY_THRESHOLD) {
	  return false;
	}
      }
      else{
	return false;
      }
    }
  }
  {
    ImageHandler screen = GetScreen();
    std::tuple<int, int, int> whitergb = screen.Crop(REFERENCE_CAPTURED_WHITE_XCOOR*SCALE_X, REFERENCE_CAPTURED_WHITE_YCOOR*SCALE_Y, REFERENCE_CAPTURED_WHITE_WIDTH*SCALE_X, REFERENCE_CAPTURED_WHITE_HEIGHT*SCALE_Y).MostCommonRGB();
    CAPTURED_WHITE_R = std::get<0>(whitergb);
    CAPTURED_WHITE_G = std::get<1>(whitergb);
    CAPTURED_WHITE_B = std::get<2>(whitergb);
  }
  return true;
}

std::vector<int> ZeldaImageProcessor::BoundingBox(std::vector<std::pair<int,int>> coordinates) {
  int top = INT_MAX;
  int bottom = 0;
  int left = INT_MAX;
  int right = 0;
  for (auto coor : coordinates) {
    if (coor.first < left) {
      left = coor.first;
    }
    if (coor.first > right) {
      right = coor.first;
    }
    if (coor.second < top) {
      top = coor.second;
    }
    if (coor.second > bottom) {
      bottom = coor.second;
    }
  }
  std::vector<int> box;
  box.push_back(left);
  box.push_back(top);
  box.push_back(right - left + 1);
  box.push_back(bottom - top + 1);
  return box;
}

std::vector<std::vector<int>> ZeldaImageProcessor::DetectBlobs(std::vector<std::pair<int,int>> coordinates, int width, int height) {
  std::vector<std::vector<int>> boxes;
  std::vector<std::vector<int>> image;
  for (int y = 0; y < height; ++y) {
    std::vector<int> row;    
    for (int x = 0; x < width; ++x) {
      row.push_back(0);
    }
    image.push_back(row);
  }
  for (auto pair : coordinates) {
    image[pair.second][pair.first] = 1;
  }

  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      if (image[y][x] == 1) {
	image[y][x] = 0;
	int t = height;
	int l = width;
	int b = 0;
	int r = 0;
	std::stack<std::pair<int, int>> graph;
	graph.push(std::make_pair(x,y));
	while (!graph.empty()) {
	  std::pair<int, int> vertex = graph.top();
	  graph.pop();
	  {
	    int newx = vertex.first-1;
	    int newy = vertex.second;
	    bool valid = newx >=0 && newx < width && newy >= 0 && newy < height;
	    if (valid && image[newy][newx] == 1) {
	      image[newy][newx] = 0;
	      graph.push(std::make_pair(newx, newy));
	      if (newx < l) l = newx;
	      if (newx > r) r = newx;
	      if (newy < t) t = newy;
	      if (newy > b) b = newy;
	    }
	  }
	  {
	    int newx = vertex.first+1;
	    int newy = vertex.second;
	    bool valid = newx >=0 && newx < width && newy >= 0 && newy < height;
	    if (valid && image[newy][newx] == 1) {
	      image[newy][newx] = 0;
	      graph.push(std::make_pair(newx, newy));
	      if (newx < l) l = newx;
	      if (newx > r) r = newx;
	      if (newy < t) t = newy;
	      if (newy > b) b = newy;
	    }
	  }
	  {
	    int newx = vertex.first;
	    int newy = vertex.second-1;
	    bool valid = newx >=0 && newx < width && newy >= 0 && newy < height;
	    if (valid && image[newy][newx] == 1) {
	      image[newy][newx] = 0;
	      graph.push(std::make_pair(newx, newy));
	      if (newx < l) l = newx;
	      if (newx > r) r = newx;
	      if (newy < t) t = newy;
	      if (newy > b) b = newy;
	    }
	  }
	  {
	    int newx = vertex.first;
	    int newy = vertex.second+1;
	    bool valid = newx >=0 && newx < width && newy >= 0 && newy < height;
	    if (valid && image[newy][newx] == 1) {
	      image[newy][newx] = 0;
	      graph.push(std::make_pair(newx, newy));
	      if (newx < l) l = newx;
	      if (newx > r) r = newx;
	      if (newy < t) t = newy;
	      if (newy > b) b = newy;
	    }
	  }	  
	}
	std::vector<int> box;
	box.push_back(l);
	box.push_back(t);
	box.push_back(r-l+1);
	box.push_back(b-t+1);
	boxes.push_back(box);
      }
    }
  }
  
  return boxes;
}

void ZeldaImageProcessor::RecordNormalDoors(ImageHandler& screen, int mapx, int mapy) {
  {
    ImageHandler dl = screen.Crop(REFERENCE_LEFT_DUNGEON_DOOR_XCOOR*SCALE_X, REFERENCE_LEFT_DUNGEON_DOOR_YCOOR*SCALE_Y, REFERENCE_LEFT_DUNGEON_DOOR_WIDTH*SCALE_X, REFERENCE_LEFT_DUNGEON_DOOR_HEIGHT*SCALE_Y);
    Dungeon::DoorType bestFit;
    double bestSim = 0;
    {
      ImageHandler dld = dl.FilterRGB(BLACK_R, BLACK_G, BLACK_B, CAPTURED_DUNGEON_DOOR_HANDLE_COLOR_TOLERANCE);
      ImageHandler zeldaDLD = dungeonld;
      double simDLD = dld.Similarity(zeldaDLD);
      if (simDLD > bestSim) {
	bestSim = simDLD;
	bestFit = Dungeon::DoorType::OPEN;
      }
    }
    {
      std::tuple<int, int, int> ccolor = dl.MostCommonRGB();            
      ImageHandler dld = dl.FilterRGB(std::get<0>(ccolor), std::get<1>(ccolor), std::get<2>(ccolor), CAPTURED_DUNGEON_DOOR_HANDLE_COLOR_TOLERANCE);
      std::tuple<int, int, int> rcolor = dungeonlk.MostCommonRGB();                  
      ImageHandler zeldaDLD = dungeonlk.FilterRGB(std::get<0>(rcolor), std::get<1>(rcolor), std::get<2>(rcolor));
      double simDLD = dld.Similarity(zeldaDLD);
      if (simDLD > bestSim) {
	bestSim = simDLD;
	bestFit = Dungeon::DoorType::KEY;
      }
    }
    {
      std::tuple<int, int, int> ccolor = dl.MostCommonRGB();
      ImageHandler dld = dl.FilterRGB(std::get<0>(ccolor), std::get<1>(ccolor), std::get<2>(ccolor), CAPTURED_DUNGEON_DOOR_HANDLE_COLOR_TOLERANCE);
      std::tuple<int, int, int> rcolor = dungeonls.MostCommonRGB();
      ImageHandler zeldaDLD = dungeonls.FilterRGB(std::get<0>(rcolor), std::get<1>(rcolor), std::get<2>(rcolor));
      double simDLD = dld.Similarity(zeldaDLD);
      if (simDLD > bestSim) {
	bestSim = simDLD;
	bestFit = Dungeon::DoorType::SHUTTER;
      }
    }
    {
      ImageHandler dld = dl.FilterRGB(BLACK_R, BLACK_G, BLACK_B, CAPTURED_DUNGEON_DOOR_HANDLE_COLOR_TOLERANCE);
      ImageHandler zeldaDLD = dungeonlb;
      double simDLD = dld.Similarity(zeldaDLD);
      if (simDLD > bestSim) {
	bestSim = simDLD;
	bestFit = Dungeon::DoorType::BOMB;
      }
    }
    {
      //prevent false positives
      double bp = static_cast<double>(dl.PixelsWithRGB(BLACK_R, BLACK_G, BLACK_B).size()) / (dl.Width() * dl.Height());
      if (bp > bestSim) {
	bestSim = 0;
      }
    }
    if (bestSim > CAPTURED_DUNGEON_ROOM_SIMILARITY_THRESHOLD &&
	!((bestSim < CAPTURED_DUNGEON_BOMB_ROOM_SIMILARITY_THRESHOLD) && (bestFit == Dungeon::DoorType::BOMB || bestFit == Dungeon::DoorType::OPEN))) {
      ZeldaInformationHandler::SetDungeonLocation(mapx-1, mapy, Dungeon::RoomType::UNSEEN_ROOM);	
      ZeldaInformationHandler::SetDungeonDoor(mapx, mapy, mapx-1, mapy, bestFit);
    }
  }
  {
    ImageHandler dl = screen.Crop(REFERENCE_RIGHT_DUNGEON_DOOR_XCOOR*SCALE_X, REFERENCE_RIGHT_DUNGEON_DOOR_YCOOR*SCALE_Y, REFERENCE_RIGHT_DUNGEON_DOOR_WIDTH*SCALE_X, REFERENCE_RIGHT_DUNGEON_DOOR_HEIGHT*SCALE_Y);
    Dungeon::DoorType bestFit;
    double bestSim = 0;
    {
      ImageHandler dld = dl.FilterRGB(BLACK_R, BLACK_G, BLACK_B, CAPTURED_DUNGEON_DOOR_HANDLE_COLOR_TOLERANCE);
      ImageHandler zeldaDLD = dungeonrd;
      double simDLD = dld.Similarity(zeldaDLD);
      if (simDLD > bestSim) {
	bestSim = simDLD;
	bestFit = Dungeon::DoorType::OPEN;
      }
    }
    {
      std::tuple<int, int, int> ccolor = dl.MostCommonRGB();
      ImageHandler dld = dl.FilterRGB(std::get<0>(ccolor), std::get<1>(ccolor), std::get<2>(ccolor), CAPTURED_DUNGEON_DOOR_HANDLE_COLOR_TOLERANCE);
      std::tuple<int, int, int> rcolor = dungeonrk.MostCommonRGB();
      ImageHandler zeldaDLD = dungeonrk.FilterRGB(std::get<0>(rcolor), std::get<1>(rcolor), std::get<2>(rcolor));
      double simDLD = dld.Similarity(zeldaDLD);
      if (simDLD > bestSim) {
	bestSim = simDLD;
	bestFit = Dungeon::DoorType::KEY;
      }
    }
    {
      std::tuple<int, int, int> ccolor = dl.MostCommonRGB();
      ImageHandler dld = dl.FilterRGB(std::get<0>(ccolor), std::get<1>(ccolor), std::get<2>(ccolor), CAPTURED_DUNGEON_DOOR_HANDLE_COLOR_TOLERANCE);
      std::tuple<int, int, int> rcolor = dungeonrs.MostCommonRGB();
      ImageHandler zeldaDLD = dungeonrs.FilterRGB(std::get<0>(rcolor), std::get<1>(rcolor), std::get<2>(rcolor));
      double simDLD = dld.Similarity(zeldaDLD);
      if (simDLD > bestSim) {
	bestSim = simDLD;
	bestFit = Dungeon::DoorType::SHUTTER;
      }
    }
    {
      ImageHandler dld = dl.FilterRGB(BLACK_R, BLACK_G, BLACK_B, CAPTURED_DUNGEON_DOOR_HANDLE_COLOR_TOLERANCE);
      ImageHandler zeldaDLD = dungeonrb;
      double simDLD = dld.Similarity(zeldaDLD);
      if (simDLD > bestSim) {
	bestSim = simDLD;
	bestFit = Dungeon::DoorType::BOMB;
      }
    }
    {
      //prevent false positives
      double bp = static_cast<double>(dl.PixelsWithRGB(BLACK_R, BLACK_G, BLACK_B).size()) / (dl.Width() * dl.Height());
      if (bp > bestSim) {
	bestSim = 0;
      }
    }    
    if (bestSim > CAPTURED_DUNGEON_ROOM_SIMILARITY_THRESHOLD &&
	!((bestSim < CAPTURED_DUNGEON_BOMB_ROOM_SIMILARITY_THRESHOLD) && (bestFit == Dungeon::DoorType::BOMB || bestFit == Dungeon::DoorType::OPEN))) {
      ZeldaInformationHandler::SetDungeonLocation(mapx+1, mapy, Dungeon::RoomType::UNSEEN_ROOM);	
      ZeldaInformationHandler::SetDungeonDoor(mapx, mapy, mapx+1, mapy, bestFit);
    }
  }
  {
    ImageHandler dl = screen.Crop(REFERENCE_TOP_DUNGEON_DOOR_XCOOR*SCALE_X, REFERENCE_TOP_DUNGEON_DOOR_YCOOR*SCALE_Y, REFERENCE_TOP_DUNGEON_DOOR_WIDTH*SCALE_X, REFERENCE_TOP_DUNGEON_DOOR_HEIGHT*SCALE_Y);
    Dungeon::DoorType bestFit;
    double bestSim = 0;
    {
      ImageHandler dld = dl.FilterRGB(BLACK_R, BLACK_G, BLACK_B, CAPTURED_DUNGEON_DOOR_HANDLE_COLOR_TOLERANCE);
      ImageHandler zeldaDLD = dungeontd;
      double simDLD = dld.Similarity(zeldaDLD);
      if (simDLD > bestSim) {
	bestSim = simDLD;
	bestFit = Dungeon::DoorType::OPEN;
      }
    }
    {
      std::tuple<int, int, int> ccolor = dl.MostCommonRGB();
      ImageHandler dld = dl.FilterRGB(std::get<0>(ccolor), std::get<1>(ccolor), std::get<2>(ccolor), CAPTURED_DUNGEON_DOOR_HANDLE_COLOR_TOLERANCE);
      std::tuple<int, int, int> rcolor = dungeontk.MostCommonRGB();
      ImageHandler zeldaDLD = dungeontk.FilterRGB(std::get<0>(rcolor), std::get<1>(rcolor), std::get<2>(rcolor));
      double simDLD = dld.Similarity(zeldaDLD);
      if (simDLD > bestSim) {
	bestSim = simDLD;
	bestFit = Dungeon::DoorType::KEY;
      }
    }
    {
      std::tuple<int, int, int> ccolor = dl.MostCommonRGB();
      ImageHandler dld = dl.FilterRGB(std::get<0>(ccolor), std::get<1>(ccolor), std::get<2>(ccolor), CAPTURED_DUNGEON_DOOR_HANDLE_COLOR_TOLERANCE);
      std::tuple<int, int, int> rcolor = dungeonts.MostCommonRGB();
      ImageHandler zeldaDLD = dungeonts.FilterRGB(std::get<0>(rcolor), std::get<1>(rcolor), std::get<2>(rcolor));
      double simDLD = dld.Similarity(zeldaDLD);
      if (simDLD > bestSim) {
	bestSim = simDLD;
	bestFit = Dungeon::DoorType::SHUTTER;
      }
    }
    {
      ImageHandler dld = dl.FilterRGB(BLACK_R, BLACK_G, BLACK_B, CAPTURED_DUNGEON_DOOR_HANDLE_COLOR_TOLERANCE);
      ImageHandler zeldaDLD = dungeontb;
      double simDLD = dld.Similarity(zeldaDLD);
      if (simDLD > bestSim) {
	bestSim = simDLD;
	bestFit = Dungeon::DoorType::BOMB;
      }
    }
    {
      //prevent false positives
      double bp = static_cast<double>(dl.PixelsWithRGB(BLACK_R, BLACK_G, BLACK_B).size()) / (dl.Width() * dl.Height());
      if (bp > bestSim) {
	bestSim = 0;
      }
    }    
    if (bestSim > CAPTURED_DUNGEON_ROOM_SIMILARITY_THRESHOLD &&
	!((bestSim < CAPTURED_DUNGEON_BOMB_ROOM_SIMILARITY_THRESHOLD) && (bestFit == Dungeon::DoorType::BOMB || bestFit == Dungeon::DoorType::OPEN))) {
      ZeldaInformationHandler::SetDungeonLocation(mapx, mapy-1, Dungeon::RoomType::UNSEEN_ROOM);	
      ZeldaInformationHandler::SetDungeonDoor(mapx, mapy, mapx, mapy-1, bestFit);
    }
  }
  {
    ImageHandler dl = screen.Crop(REFERENCE_BOTTOM_DUNGEON_DOOR_XCOOR*SCALE_X, REFERENCE_BOTTOM_DUNGEON_DOOR_YCOOR*SCALE_Y, REFERENCE_BOTTOM_DUNGEON_DOOR_WIDTH*SCALE_X, REFERENCE_BOTTOM_DUNGEON_DOOR_HEIGHT*SCALE_Y);
    Dungeon::DoorType bestFit;
    double bestSim = 0;
    {
      ImageHandler dld = dl.FilterRGB(BLACK_R, BLACK_G, BLACK_B, CAPTURED_DUNGEON_DOOR_HANDLE_COLOR_TOLERANCE);
      ImageHandler zeldaDLD = dungeonbd;
      double simDLD = dld.Similarity(zeldaDLD);
      if (simDLD > bestSim) {
	bestSim = simDLD;
	bestFit = Dungeon::DoorType::OPEN;
      }
    }
    {
      std::tuple<int, int, int> ccolor = dl.MostCommonRGB();
      ImageHandler dld = dl.FilterRGB(std::get<0>(ccolor), std::get<1>(ccolor), std::get<2>(ccolor), CAPTURED_DUNGEON_DOOR_HANDLE_COLOR_TOLERANCE);
      std::tuple<int, int, int> rcolor = dungeonbk.MostCommonRGB();
      ImageHandler zeldaDLD = dungeonbk.FilterRGB(std::get<0>(rcolor), std::get<1>(rcolor), std::get<2>(rcolor));
      double simDLD = dld.Similarity(zeldaDLD);
      if (simDLD > bestSim) {
	bestSim = simDLD;
	bestFit = Dungeon::DoorType::KEY;
      }
    }
    {
      std::tuple<int, int, int> ccolor = dl.MostCommonRGB();
      ImageHandler dld = dl.FilterRGB(std::get<0>(ccolor), std::get<1>(ccolor), std::get<2>(ccolor), CAPTURED_DUNGEON_DOOR_HANDLE_COLOR_TOLERANCE);
      std::tuple<int, int, int> rcolor = dungeonbs.MostCommonRGB();
      ImageHandler zeldaDLD = dungeonbs.FilterRGB(std::get<0>(rcolor), std::get<1>(rcolor), std::get<2>(rcolor));
      double simDLD = dld.Similarity(zeldaDLD);
      if (simDLD > bestSim) {
	bestSim = simDLD;
	bestFit = Dungeon::DoorType::SHUTTER;
      }
    }
    {
      ImageHandler dld = dl.FilterRGB(BLACK_R, BLACK_G, BLACK_B, CAPTURED_DUNGEON_DOOR_HANDLE_COLOR_TOLERANCE);
      ImageHandler zeldaDLD = dungeonbb;
      double simDLD = dld.Similarity(zeldaDLD);
      if (simDLD > bestSim) {
	bestSim = simDLD;
	bestFit = Dungeon::DoorType::BOMB;
      }
    }
    {
      //prevent false positives
      double bp = static_cast<double>(dl.PixelsWithRGB(BLACK_R, BLACK_G, BLACK_B).size()) / (dl.Width() * dl.Height());
      if (bp > bestSim) {
	bestSim = 0;
      }
    }
    if (bestSim > CAPTURED_DUNGEON_ROOM_SIMILARITY_THRESHOLD &&
	!((bestSim < CAPTURED_DUNGEON_BOMB_ROOM_SIMILARITY_THRESHOLD) && (bestFit == Dungeon::DoorType::BOMB || bestFit == Dungeon::DoorType::OPEN))) {
      ZeldaInformationHandler::SetDungeonLocation(mapx, mapy+1, Dungeon::RoomType::UNSEEN_ROOM);	
      ZeldaInformationHandler::SetDungeonDoor(mapx, mapy, mapx, mapy+1, bestFit);
    }
  }
}

void ZeldaImageProcessor::RecordDarkDoors(ImageHandler& screen, int mapx, int mapy) {
  ImageHandler handle = screen.Crop(REFERENCE_DUNGEON_DOOR_HANDLE_XCOOR*SCALE_X, REFERENCE_DUNGEON_DOOR_HANDLE_YCOOR*SCALE_Y, REFERENCE_DUNGEON_DOOR_HANDLE_WIDTH*SCALE_X, REFERENCE_DUNGEON_DOOR_HANDLE_HEIGHT*SCALE_Y);
  std::tuple<int, int, int> hcolor = handle.MostCommonRGB();
  {
    ImageHandler dl = screen.Crop(REFERENCE_LEFT_DUNGEON_DOOR_XCOOR*SCALE_X, REFERENCE_LEFT_DUNGEON_DOOR_YCOOR*SCALE_Y, REFERENCE_LEFT_DUNGEON_DOOR_WIDTH*SCALE_X, REFERENCE_LEFT_DUNGEON_DOOR_HEIGHT*SCALE_Y);
    dl = dl.FilterRGB(std::get<0>(hcolor), std::get<1>(hcolor), std::get<2>(hcolor), CAPTURED_DUGEON_ROOM_COLOR_TOLERANCE);
    dl = dl.FilterRGB(WHITE_R, WHITE_G, WHITE_B);
    Dungeon::DoorType bestFit;
    double bestSim = 0;
    {
      ImageHandler dld = dl;
      ImageHandler zeldaDLD = dungeondld;
      double simDLD = dld.Similarity(zeldaDLD);
      simDLD = std::max(simDLD, 1 - simDLD);
      if (simDLD > bestSim) {
	bestSim = simDLD;
	bestFit = Dungeon::DoorType::OPEN;
      }
    }
    {
      ImageHandler dld = dl;
      ImageHandler zeldaDLD = dungeondlk;
      double simDLD = dld.Similarity(zeldaDLD);
      simDLD = std::max(simDLD, 1 - simDLD);
      if (simDLD > bestSim) {
	bestSim = simDLD;
	bestFit = Dungeon::DoorType::KEY;
      }
    }
    {
      ImageHandler dld = dl;
      ImageHandler zeldaDLD = dungeondls;
      double simDLD = dld.Similarity(zeldaDLD);
      simDLD = std::max(simDLD, 1 - simDLD);
      if (simDLD > bestSim) {
	bestSim = simDLD;
	bestFit = Dungeon::DoorType::SHUTTER;
      }
    }
    {
      ImageHandler dld = dl;
      ImageHandler zeldaDLD = dungeondlb;
      double simDLD = dld.Similarity(zeldaDLD);
      simDLD = std::max(simDLD, 1 - simDLD);
      if (simDLD > bestSim) {
	bestSim = simDLD;
	bestFit = Dungeon::DoorType::BOMB;
      }
    }
    {
      //prevent false positives
      double bp = static_cast<double>(dl.PixelsWithRGB(BLACK_R, BLACK_G, BLACK_B).size()) / (dl.Width() * dl.Height());
      if (bp > bestSim || 1-bp > bestSim) {
	bestSim = 0;
      }
    }
    if (bestSim > CAPTURED_DUNGEON_DARK_ROOM_SIMILARITY_THRESHOLD &&
	!((bestSim < CAPTURED_DUNGEON_DARK_BOMB_ROOM_SIMILARITY_THRESHOLD) && (bestFit == Dungeon::DoorType::BOMB))) {
      ZeldaInformationHandler::SetDungeonLocation(mapx-1, mapy, Dungeon::RoomType::UNSEEN_ROOM);	
      ZeldaInformationHandler::SetDungeonDoor(mapx, mapy, mapx-1, mapy, bestFit);
    }
  }
  {
    ImageHandler dl = screen.Crop(REFERENCE_RIGHT_DUNGEON_DOOR_XCOOR*SCALE_X, REFERENCE_RIGHT_DUNGEON_DOOR_YCOOR*SCALE_Y, REFERENCE_RIGHT_DUNGEON_DOOR_WIDTH*SCALE_X, REFERENCE_RIGHT_DUNGEON_DOOR_HEIGHT*SCALE_Y);
    dl = dl.FilterRGB(std::get<0>(hcolor), std::get<1>(hcolor), std::get<2>(hcolor), CAPTURED_DUGEON_ROOM_COLOR_TOLERANCE);
    dl = dl.FilterRGB(WHITE_R, WHITE_G, WHITE_B);
    Dungeon::DoorType bestFit;
    double bestSim = 0;
    {
      ImageHandler dld = dl;
      ImageHandler zeldaDLD = dungeondrd;
      double simDLD = dld.Similarity(zeldaDLD);
      simDLD = std::max(simDLD, 1 - simDLD);
      if (simDLD > bestSim) {
	bestSim = simDLD;
	bestFit = Dungeon::DoorType::OPEN;
      }
    }
    {
      ImageHandler dld = dl;
      ImageHandler zeldaDLD = dungeondrk;
      double simDLD = dld.Similarity(zeldaDLD);
      simDLD = std::max(simDLD, 1 - simDLD);
      if (simDLD > bestSim) {
	bestSim = simDLD;
	bestFit = Dungeon::DoorType::KEY;
      }
    }
    {
      ImageHandler dld = dl;
      ImageHandler zeldaDLD = dungeondrs;
      double simDLD = dld.Similarity(zeldaDLD);
      simDLD = std::max(simDLD, 1 - simDLD);
      if (simDLD > bestSim) {
	bestSim = simDLD;
	bestFit = Dungeon::DoorType::SHUTTER;
      }
    }
    {
      ImageHandler dld = dl;
      ImageHandler zeldaDLD = dungeondrb;
      double simDLD = dld.Similarity(zeldaDLD);
      simDLD = std::max(simDLD, 1 - simDLD);
      if (simDLD > bestSim) {
	bestSim = simDLD;
	bestFit = Dungeon::DoorType::BOMB;
      }
    }
    {
      //prevent false positives
      double bp = static_cast<double>(dl.PixelsWithRGB(BLACK_R, BLACK_G, BLACK_B).size()) / (dl.Width() * dl.Height());
      if (bp > bestSim || 1-bp > bestSim) {
	bestSim = 0;
      }
    }    
    if (bestSim > CAPTURED_DUNGEON_DARK_ROOM_SIMILARITY_THRESHOLD &&
	!((bestSim < CAPTURED_DUNGEON_DARK_BOMB_ROOM_SIMILARITY_THRESHOLD) && (bestFit == Dungeon::DoorType::BOMB))) {
      ZeldaInformationHandler::SetDungeonLocation(mapx+1, mapy, Dungeon::RoomType::UNSEEN_ROOM);	
      ZeldaInformationHandler::SetDungeonDoor(mapx, mapy, mapx+1, mapy, bestFit);
    }
  }
  {
    ImageHandler dl = screen.Crop(REFERENCE_TOP_DUNGEON_DOOR_XCOOR*SCALE_X, REFERENCE_TOP_DUNGEON_DOOR_YCOOR*SCALE_Y, REFERENCE_TOP_DUNGEON_DOOR_WIDTH*SCALE_X, REFERENCE_TOP_DUNGEON_DOOR_HEIGHT*SCALE_Y);
    dl = dl.FilterRGB(std::get<0>(hcolor), std::get<1>(hcolor), std::get<2>(hcolor), CAPTURED_DUGEON_ROOM_COLOR_TOLERANCE);
    dl = dl.FilterRGB(WHITE_R, WHITE_G, WHITE_B);
    Dungeon::DoorType bestFit;
    double bestSim = 0;
    {
      ImageHandler dld = dl;
      ImageHandler zeldaDLD = dungeondtd;
      double simDLD = dld.Similarity(zeldaDLD);
      simDLD = std::max(simDLD, 1 - simDLD);
      if (simDLD > bestSim) {
	bestSim = simDLD;
	bestFit = Dungeon::DoorType::OPEN;
      }
    }
    {
      ImageHandler dld = dl;
      ImageHandler zeldaDLD = dungeondtk;
      double simDLD = dld.Similarity(zeldaDLD);
      simDLD = std::max(simDLD, 1 - simDLD);
      if (simDLD > bestSim) {
	bestSim = simDLD;
	bestFit = Dungeon::DoorType::KEY;
      }
    }
    {
      ImageHandler dld = dl;
      ImageHandler zeldaDLD = dungeondts;
      double simDLD = dld.Similarity(zeldaDLD);
      simDLD = std::max(simDLD, 1 - simDLD);
      if (simDLD > bestSim) {
	bestSim = simDLD;
	bestFit = Dungeon::DoorType::SHUTTER;
      }
    }
    {
      ImageHandler dld = dl;
      ImageHandler zeldaDLD = dungeondtb;
      double simDLD = dld.Similarity(zeldaDLD);
      simDLD = std::max(simDLD, 1 - simDLD);
      if (simDLD > bestSim) {
	bestSim = simDLD;
	bestFit = Dungeon::DoorType::BOMB;
      }
    }
    {
      //prevent false positives
      double bp = static_cast<double>(dl.PixelsWithRGB(BLACK_R, BLACK_G, BLACK_B).size()) / (dl.Width() * dl.Height());
      if (bp > bestSim || 1-bp > bestSim) {
	bestSim = 0;
      }
    }    
    if (bestSim > CAPTURED_DUNGEON_DARK_ROOM_SIMILARITY_THRESHOLD &&
	!((bestSim < CAPTURED_DUNGEON_DARK_BOMB_ROOM_SIMILARITY_THRESHOLD) && (bestFit == Dungeon::DoorType::BOMB))) {
      ZeldaInformationHandler::SetDungeonLocation(mapx, mapy-1, Dungeon::RoomType::UNSEEN_ROOM);	
      ZeldaInformationHandler::SetDungeonDoor(mapx, mapy, mapx, mapy-1, bestFit);
    }
  }
  {
    ImageHandler dl = screen.Crop(REFERENCE_BOTTOM_DUNGEON_DOOR_XCOOR*SCALE_X, REFERENCE_BOTTOM_DUNGEON_DOOR_YCOOR*SCALE_Y, REFERENCE_BOTTOM_DUNGEON_DOOR_WIDTH*SCALE_X, REFERENCE_BOTTOM_DUNGEON_DOOR_HEIGHT*SCALE_Y);
    dl = dl.FilterRGB(std::get<0>(hcolor), std::get<1>(hcolor), std::get<2>(hcolor), CAPTURED_DUGEON_ROOM_COLOR_TOLERANCE);
    dl = dl.FilterRGB(WHITE_R, WHITE_G, WHITE_B);
    Dungeon::DoorType bestFit;
    double bestSim = 0;
    {
      ImageHandler dld = dl;
      ImageHandler zeldaDLD = dungeondbd;
      double simDLD = dld.Similarity(zeldaDLD);
      simDLD = std::max(simDLD, 1 - simDLD);
      if (simDLD > bestSim) {
	bestSim = simDLD;
	bestFit = Dungeon::DoorType::OPEN;
      }
    }
    {
      ImageHandler dld = dl;
      ImageHandler zeldaDLD = dungeondbk;
      double simDLD = dld.Similarity(zeldaDLD);
      simDLD = std::max(simDLD, 1 - simDLD);
      if (simDLD > bestSim) {
	bestSim = simDLD;
	bestFit = Dungeon::DoorType::KEY;
      }
    }
    {
      ImageHandler dld = dl;
      ImageHandler zeldaDLD = dungeondbs;
      double simDLD = dld.Similarity(zeldaDLD);
      simDLD = std::max(simDLD, 1 - simDLD);
      if (simDLD > bestSim) {
	bestSim = simDLD;
	bestFit = Dungeon::DoorType::SHUTTER;
      }
    }
    {
      ImageHandler dld = dl;
      ImageHandler zeldaDLD = dungeondbb;
      double simDLD = dld.Similarity(zeldaDLD);
      simDLD = std::max(simDLD, 1 - simDLD);
      if (simDLD > bestSim) {
	bestSim = simDLD;
	bestFit = Dungeon::DoorType::BOMB;
      }
    }
    {
      //prevent false positives
      double bp = static_cast<double>(dl.PixelsWithRGB(BLACK_R, BLACK_G, BLACK_B).size()) / (dl.Width() * dl.Height());
      if (bp > bestSim || 1-bp > bestSim) {
	bestSim = 0;
      }
    }
    if (bestSim > CAPTURED_DUNGEON_DARK_ROOM_SIMILARITY_THRESHOLD &&
	!((bestSim < CAPTURED_DUNGEON_DARK_BOMB_ROOM_SIMILARITY_THRESHOLD) && (bestFit == Dungeon::DoorType::BOMB))) {
      ZeldaInformationHandler::SetDungeonLocation(mapx, mapy+1, Dungeon::RoomType::UNSEEN_ROOM);	
      ZeldaInformationHandler::SetDungeonDoor(mapx, mapy, mapx, mapy+1, bestFit);
    }
  }
}

void ZeldaImageProcessor::RecordBlackDoors(ImageHandler& screen, int mapx, int mapy) {
  {
    ImageHandler dl = screen.Crop(REFERENCE_LEFT_DUNGEON_DOOR_XCOOR*SCALE_X, REFERENCE_LEFT_DUNGEON_DOOR_YCOOR*SCALE_Y, REFERENCE_LEFT_DUNGEON_DOOR_WIDTH*SCALE_X, REFERENCE_LEFT_DUNGEON_DOOR_HEIGHT*SCALE_Y);
    Dungeon::DoorType bestFit;
    double bestSim = 0;
    {
      ImageHandler dld = dl.FilterRGB(BLACK_R, BLACK_G, BLACK_B, CAPTURED_DUGEON_ROOM_COLOR_TOLERANCE);
      ImageHandler zeldaDLD = dungeonbld;
      double simDLD = dld.Similarity(zeldaDLD);
      if (simDLD > bestSim) {
	bestSim = simDLD;
	bestFit = Dungeon::DoorType::OPEN;
      }
    }
    {
      ImageHandler dld = dl.FilterRGB(BLACK_R, BLACK_G, BLACK_B, CAPTURED_DUGEON_ROOM_COLOR_TOLERANCE);
      ImageHandler zeldaDLD = dungeonblk;
      double simDLD = dld.Similarity(zeldaDLD);
      if (simDLD > bestSim) {
	bestSim = simDLD;
	bestFit = Dungeon::DoorType::KEY;
      }
    }
    {
      ImageHandler dld = dl.FilterRGB(BLACK_R, BLACK_G, BLACK_B, CAPTURED_DUGEON_ROOM_COLOR_TOLERANCE);
      ImageHandler zeldaDLD = dungeonbls;
      double simDLD = dld.Similarity(zeldaDLD);
      if (simDLD > bestSim) {
	bestSim = simDLD;
	bestFit = Dungeon::DoorType::SHUTTER;
      }
    }
    {
      ImageHandler dld = dl.FilterRGB(BLACK_R, BLACK_G, BLACK_B, CAPTURED_DUGEON_ROOM_COLOR_TOLERANCE);
      ImageHandler zeldaDLD = dungeonblb;
      double simDLD = dld.Similarity(zeldaDLD);
      if (simDLD > bestSim) {
	bestSim = simDLD;
	bestFit = Dungeon::DoorType::BOMB;
      }
    }
    {
      //prevent false positives
      double bp = static_cast<double>(dl.PixelsWithRGB(BLACK_R, BLACK_G, BLACK_B).size()) / (dl.Width() * dl.Height());
      if (bp > bestSim || 1 - bp > bestSim) {
	bestSim = 0;
      }
    }
    if (bestSim > CAPTURED_DUNGEON_BLACK_ROOM_SIMILARITY_THRESHOLD &&
	!((bestSim < CAPTURED_DUNGEON_BLACK_BOMB_ROOM_SIMILARITY_THRESHOLD) && (bestFit == Dungeon::DoorType::BOMB))) {
      ZeldaInformationHandler::SetDungeonLocation(mapx - 1, mapy, Dungeon::RoomType::UNSEEN_ROOM);
      ZeldaInformationHandler::SetDungeonDoor(mapx, mapy, mapx - 1, mapy, bestFit);
    }
  }
  {
    ImageHandler dl = screen.Crop(REFERENCE_RIGHT_DUNGEON_DOOR_XCOOR*SCALE_X, REFERENCE_RIGHT_DUNGEON_DOOR_YCOOR*SCALE_Y, REFERENCE_RIGHT_DUNGEON_DOOR_WIDTH*SCALE_X, REFERENCE_RIGHT_DUNGEON_DOOR_HEIGHT*SCALE_Y);
    Dungeon::DoorType bestFit;
    double bestSim = 0;
    {
      ImageHandler dld = dl.FilterRGB(BLACK_R, BLACK_G, BLACK_B, CAPTURED_DUGEON_ROOM_COLOR_TOLERANCE);
      ImageHandler zeldaDLD = dungeonbrd;
      double simDLD = dld.Similarity(zeldaDLD);
      if (simDLD > bestSim) {
	bestSim = simDLD;
	bestFit = Dungeon::DoorType::OPEN;
      }
    }
    {
      ImageHandler dld = dl.FilterRGB(BLACK_R, BLACK_G, BLACK_B, CAPTURED_DUGEON_ROOM_COLOR_TOLERANCE);
      ImageHandler zeldaDLD = dungeonbrk;
      double simDLD = dld.Similarity(zeldaDLD);
      if (simDLD > bestSim) {
	bestSim = simDLD;
	bestFit = Dungeon::DoorType::KEY;
      }
    }
    {
      ImageHandler dld = dl.FilterRGB(BLACK_R, BLACK_G, BLACK_B, CAPTURED_DUGEON_ROOM_COLOR_TOLERANCE);
      ImageHandler zeldaDLD = dungeonbrs;
      double simDLD = dld.Similarity(zeldaDLD);
      if (simDLD > bestSim) {
	bestSim = simDLD;
	bestFit = Dungeon::DoorType::SHUTTER;
      }
    }
    {
      ImageHandler dld = dl.FilterRGB(BLACK_R, BLACK_G, BLACK_B, CAPTURED_DUGEON_ROOM_COLOR_TOLERANCE);
      ImageHandler zeldaDLD = dungeonbrb;
      double simDLD = dld.Similarity(zeldaDLD);
      if (simDLD > bestSim) {
	bestSim = simDLD;
	bestFit = Dungeon::DoorType::BOMB;
      }
    }
    {
      //prevent false positives
      double bp = static_cast<double>(dl.PixelsWithRGB(BLACK_R, BLACK_G, BLACK_B).size()) / (dl.Width() * dl.Height());
      if (bp > bestSim || 1 - bp > bestSim) {
	bestSim = 0;
      }
    }
    if (bestSim > CAPTURED_DUNGEON_BLACK_ROOM_SIMILARITY_THRESHOLD &&
	!((bestSim < CAPTURED_DUNGEON_BLACK_BOMB_ROOM_SIMILARITY_THRESHOLD) && (bestFit == Dungeon::DoorType::BOMB))) {
      ZeldaInformationHandler::SetDungeonLocation(mapx + 1, mapy, Dungeon::RoomType::UNSEEN_ROOM);
      ZeldaInformationHandler::SetDungeonDoor(mapx, mapy, mapx + 1, mapy, bestFit);
    }
  }
  {
    ImageHandler dl = screen.Crop(REFERENCE_TOP_DUNGEON_DOOR_XCOOR*SCALE_X, REFERENCE_TOP_DUNGEON_DOOR_YCOOR*SCALE_Y, REFERENCE_TOP_DUNGEON_DOOR_WIDTH*SCALE_X, REFERENCE_TOP_DUNGEON_DOOR_HEIGHT*SCALE_Y);
    Dungeon::DoorType bestFit;
    double bestSim = 0;
    {
      ImageHandler dld = dl.FilterRGB(BLACK_R, BLACK_G, BLACK_B, CAPTURED_DUGEON_ROOM_COLOR_TOLERANCE);
      ImageHandler zeldaDLD = dungeonbtd;
      double simDLD = dld.Similarity(zeldaDLD);
      if (simDLD > bestSim) {
	bestSim = simDLD;
	bestFit = Dungeon::DoorType::OPEN;
      }
    }
    {
      ImageHandler dld = dl.FilterRGB(BLACK_R, BLACK_G, BLACK_B, CAPTURED_DUGEON_ROOM_COLOR_TOLERANCE);
      ImageHandler zeldaDLD = dungeonbtk;
      double simDLD = dld.Similarity(zeldaDLD);
      if (simDLD > bestSim) {
	bestSim = simDLD;
	bestFit = Dungeon::DoorType::KEY;
      }
    }
    {
      ImageHandler dld = dl.FilterRGB(BLACK_R, BLACK_G, BLACK_B, CAPTURED_DUGEON_ROOM_COLOR_TOLERANCE);
      ImageHandler zeldaDLD = dungeonbts;
      double simDLD = dld.Similarity(zeldaDLD);
      if (simDLD > bestSim) {
	bestSim = simDLD;
	bestFit = Dungeon::DoorType::SHUTTER;
      }
    }
    {
      ImageHandler dld = dl.FilterRGB(BLACK_R, BLACK_G, BLACK_B, CAPTURED_DUGEON_ROOM_COLOR_TOLERANCE);
      ImageHandler zeldaDLD = dungeonbtb;
      double simDLD = dld.Similarity(zeldaDLD);
      if (simDLD > bestSim) {
	bestSim = simDLD;
	bestFit = Dungeon::DoorType::BOMB;
      }
    }
    {
      //prevent false positives
      double bp = static_cast<double>(dl.PixelsWithRGB(BLACK_R, BLACK_G, BLACK_B).size()) / (dl.Width() * dl.Height());
      if (bp > bestSim || 1 - bp > bestSim) {
	bestSim = 0;
      }
    }
    if (bestSim > CAPTURED_DUNGEON_BLACK_ROOM_SIMILARITY_THRESHOLD &&
	!((bestSim < CAPTURED_DUNGEON_BLACK_BOMB_ROOM_SIMILARITY_THRESHOLD) && (bestFit == Dungeon::DoorType::BOMB))) {
      ZeldaInformationHandler::SetDungeonLocation(mapx, mapy - 1, Dungeon::RoomType::UNSEEN_ROOM);
      ZeldaInformationHandler::SetDungeonDoor(mapx, mapy, mapx, mapy - 1, bestFit);
    }
  }
  {
    ImageHandler dl = screen.Crop(REFERENCE_BOTTOM_DUNGEON_DOOR_XCOOR*SCALE_X, REFERENCE_BOTTOM_DUNGEON_DOOR_YCOOR*SCALE_Y, REFERENCE_BOTTOM_DUNGEON_DOOR_WIDTH*SCALE_X, REFERENCE_BOTTOM_DUNGEON_DOOR_HEIGHT*SCALE_Y);
    Dungeon::DoorType bestFit;
    double bestSim = 0;
    {
      ImageHandler dld = dl.FilterRGB(BLACK_R, BLACK_G, BLACK_B, CAPTURED_DUGEON_ROOM_COLOR_TOLERANCE);
      ImageHandler zeldaDLD = dungeonbbd;
      double simDLD = dld.Similarity(zeldaDLD);
      if (simDLD > bestSim) {
	bestSim = simDLD;
	bestFit = Dungeon::DoorType::OPEN;
      }
    }
    {
      ImageHandler dld = dl.FilterRGB(BLACK_R, BLACK_G, BLACK_B, CAPTURED_DUGEON_ROOM_COLOR_TOLERANCE);
      ImageHandler zeldaDLD = dungeonbbk;
      double simDLD = dld.Similarity(zeldaDLD);
      if (simDLD > bestSim) {
	bestSim = simDLD;
	bestFit = Dungeon::DoorType::KEY;
      }
    }
    {
      ImageHandler dld = dl.FilterRGB(BLACK_R, BLACK_G, BLACK_B, CAPTURED_DUGEON_ROOM_COLOR_TOLERANCE);
      ImageHandler zeldaDLD = dungeonbbs;
      double simDLD = dld.Similarity(zeldaDLD);
      if (simDLD > bestSim) {
	bestSim = simDLD;
	bestFit = Dungeon::DoorType::SHUTTER;
      }
    }
    {
      ImageHandler dld = dl.FilterRGB(BLACK_R, BLACK_G, BLACK_B, CAPTURED_DUGEON_ROOM_COLOR_TOLERANCE);
      ImageHandler zeldaDLD = dungeonbbb;
      double simDLD = dld.Similarity(zeldaDLD);
      if (simDLD > bestSim) {
	bestSim = simDLD;
	bestFit = Dungeon::DoorType::BOMB;
      }
    }
    {
      //prevent false positives
      double bp = static_cast<double>(dl.PixelsWithRGB(BLACK_R, BLACK_G, BLACK_B).size()) / (dl.Width() * dl.Height());
      if (bp > bestSim || 1 - bp > bestSim) {
	bestSim = 0;
      }
    }
    if (bestSim > CAPTURED_DUNGEON_BLACK_ROOM_SIMILARITY_THRESHOLD &&
	!((bestSim < CAPTURED_DUNGEON_BLACK_BOMB_ROOM_SIMILARITY_THRESHOLD) && (bestFit == Dungeon::DoorType::BOMB))) {
      ZeldaInformationHandler::SetDungeonLocation(mapx, mapy + 1, Dungeon::RoomType::UNSEEN_ROOM);
      ZeldaInformationHandler::SetDungeonDoor(mapx, mapy, mapx, mapy + 1, bestFit);
    }
  }
}

void ZeldaImageProcessor::CheckSecretCave(ImageHandler& screen, int mapx, int mapy) {
  ImageHandler playScreen = screen.Crop(REFERENCE_PLAYING_SCREEN_XCOOR*SCALE_X, REFERENCE_PLAYING_SCREEN_YCOOR*SCALE_Y, REFERENCE_PLAYING_SCREEN_WIDTH*SCALE_X, REFERENCE_PLAYING_SCREEN_HEIGHT*SCALE_Y);
  double bp = static_cast<double>(playScreen.PixelsWithRGB(BLACK_R, BLACK_G, BLACK_B).size()) / (playScreen.Width() * playScreen.Height());	      
  ImageHandler shopOwner = screen.Crop(REFERENCE_SHOP_OWNER_XCOOR*SCALE_X, REFERENCE_SHOP_OWNER_YCOOR*SCALE_Y, REFERENCE_SHOP_OWNER_WIDTH*SCALE_X, REFERENCE_SHOP_OWNER_HEIGHT*SCALE_Y);
  double sobp = static_cast<double>(shopOwner.PixelsWithRGB(BLACK_R, BLACK_G, BLACK_B).size()) / (shopOwner.Width() * shopOwner.Height());
  bool inSecretCave = (bp > SECRET_CAVE_BLACK_THRESHOLD) && (sobp < SHOP_OWNER_BLACK_THRESHOLD) && (playScreen.PixelsWithRGB(CURRENT_TUNIC_R, CURRENT_TUNIC_G, CURRENT_TUNIC_B).size() != 0);
  if (!inSecretCave) {
    return;
  }
  
  bool foundSecret = false;  
  //check if in a potion shop before showing letter. Check this case first because it is the easiest to mislabel
  {
    ImageHandler item = screen.Crop(REFERENCE_POTION_SHOP_TEXT_XCOOR*SCALE_X, REFERENCE_POTION_SHOP_TEXT_YCOOR*SCALE_Y, REFERENCE_POTION_SHOP_TEXT_WIDTH*SCALE_X, REFERENCE_POTION_SHOP_TEXT_HEIGHT*SCALE_Y);
    //If letter has not been shown, then potion shop will have no text
    ZeldaInformationHandler::Secrets prev = ZeldaInformationHandler::GetSecret(mapx, mapy);
    if ((prev == ZeldaInformationHandler::Secrets::UNKNOWN_CAVE) || (prev == ZeldaInformationHandler::Secrets::UNEXPLORED) || (prev == ZeldaInformationHandler::Secrets::PRE_POTION_SHOP)) {
      if (item.PixelsWithRGB(CAPTURED_WHITE_R, CAPTURED_WHITE_G, CAPTURED_WHITE_B).size() == 0) {
	ZeldaInformationHandler::SetSecret(mapx, mapy, ZeldaInformationHandler::Secrets::PRE_POTION_SHOP);
	foundSecret = true;
      }
      else {
	ZeldaInformationHandler::SetSecret(mapx, mapy, ZeldaInformationHandler::Secrets::EXPLORED_CAVE);
	foundSecret = true;
      }
    }
  }
  //check if in anyroad
  {
    ImageHandler road = screen.Crop(REFERENCE_ANYROAD_XCOOR*SCALE_X, REFERENCE_ANYROAD_YCOOR*SCALE_Y, REFERENCE_ANYROAD_WIDTH*SCALE_X, REFERENCE_ANYROAD_HEIGHT*SCALE_Y).FilterRGB(CAPTURED_WHITE_R, CAPTURED_WHITE_G, CAPTURED_WHITE_B, CAPTURED_ANYROAD_COLOR_TOLERANCE);
    if (road.Similarity(anyroad) > CAPTURED_ANYROAD_SIMILARITY_THRESHOLD) {
      ZeldaInformationHandler::SetSecret(mapx, mapy, ZeldaInformationHandler::Secrets::ANYROAD);
      foundSecret = true;
    }
  }
  //check if in sword cave
  {
    bool foundWhiteSword = false;
    {
      ImageHandler sword = screen.Crop(REFERENCE_WHITE_SWORD_XCOOR*SCALE_X, REFERENCE_WHITE_SWORD_YCOOR*SCALE_Y, REFERENCE_WHITE_SWORD_WIDTH*SCALE_X, REFERENCE_WHITE_SWORD_HEIGHT*SCALE_Y).FilterRGB(CAPTURED_WHITE_R, CAPTURED_WHITE_G, CAPTURED_WHITE_B, CAPTURED_ITEM_COLOR_TOLERANCE);
      if (sword.Similarity(whitesword) > CAPTURED_SWORD_SIMILARITY_THRESHOLD) {
	ZeldaInformationHandler::SetSecret(mapx, mapy, ZeldaInformationHandler::Secrets::WHITE_SWORD);
	foundSecret = true;
	foundWhiteSword = true;
      }
    }
    {
      if (!foundWhiteSword) {
	ImageHandler sword = screen.Crop(REFERENCE_MAGICAL_SWORD_XCOOR*SCALE_X, REFERENCE_MAGICAL_SWORD_YCOOR*SCALE_Y, REFERENCE_MAGICAL_SWORD_WIDTH*SCALE_X, REFERENCE_MAGICAL_SWORD_HEIGHT*SCALE_Y).FilterRGB(CAPTURED_WHITE_R, CAPTURED_WHITE_G, CAPTURED_WHITE_B, CAPTURED_DUGEON_ROOM_COLOR_TOLERANCE);
	if (sword.Similarity(magicalsword) > CAPTURED_SWORD_SIMILARITY_THRESHOLD) {
	  ZeldaInformationHandler::SetSecret(mapx, mapy, ZeldaInformationHandler::Secrets::MAGICAL_SWORD);
	  foundSecret = true;
	}
      }
    }
  }
  //check if in a shop
  bool lpotionshop = false;
  bool rpotionshop = false;
  {
    ImageHandler item = screen.Crop(REFERENCE_SHOP_LEFT_ITEM_XCOOR*SCALE_X, REFERENCE_SHOP_LEFT_ITEM_YCOOR*SCALE_Y, REFERENCE_SHOP_LEFT_ITEM_WIDTH*SCALE_X, REFERENCE_SHOP_LEFT_ITEM_HEIGHT*SCALE_Y).FilterRGB(BLACK_R, BLACK_G, BLACK_B, CAPTURED_ITEM_COLOR_TOLERANCE);
    if (item.Similarity(bluecandle) > CAPTURED_SHOP_ITEM_SIMILARITY_THRESHOLD) {
      ZeldaInformationHandler::SetSecret(mapx, mapy, ZeldaInformationHandler::Secrets::CANDLE_SHOP);
      foundSecret = true;
    }
    if (item.Similarity(bomb) > CAPTURED_SHOP_ITEM_SIMILARITY_THRESHOLD) {
      ZeldaInformationHandler::SetSecret(mapx, mapy, ZeldaInformationHandler::Secrets::BOMB_SHOP);
      foundSecret = true;
    }		  
    if (item.Similarity(arrow) > CAPTURED_SHOP_ITEM_SIMILARITY_THRESHOLD) {
      ZeldaInformationHandler::SetSecret(mapx, mapy, ZeldaInformationHandler::Secrets::ARROW_SHOP);
      foundSecret = true;
    }
    if (item.Similarity(bait) > CAPTURED_SHOP_ITEM_SIMILARITY_THRESHOLD) {
      ZeldaInformationHandler::SetSecret(mapx, mapy, ZeldaInformationHandler::Secrets::BAIT_SHOP);
      foundSecret = true;
    }
    if (item.Similarity(bluering) > CAPTURED_SHOP_ITEM_SIMILARITY_THRESHOLD) {
      ZeldaInformationHandler::SetSecret(mapx, mapy, ZeldaInformationHandler::Secrets::BLUE_RING_SHOP);
      foundSecret = true;
    }
    if (item.Similarity(woodsword) > CAPTURED_SHOP_ITEM_SIMILARITY_THRESHOLD) {
      ZeldaInformationHandler::SetSecret(mapx, mapy, ZeldaInformationHandler::Secrets::WOOD_SWORD);
      foundSecret = true;
    }
    if (item.Similarity(potion) > CAPTURED_SHOP_ITEM_SIMILARITY_THRESHOLD) {
      lpotionshop = true;
    }		  
  }
  {
    ImageHandler item = screen.Crop(REFERENCE_SHOP_MIDDLE_ITEM_XCOOR*SCALE_X, REFERENCE_SHOP_MIDDLE_ITEM_YCOOR*SCALE_Y, REFERENCE_SHOP_MIDDLE_ITEM_WIDTH*SCALE_X, REFERENCE_SHOP_MIDDLE_ITEM_HEIGHT*SCALE_Y).FilterRGB(BLACK_R, BLACK_G, BLACK_B, CAPTURED_ITEM_COLOR_TOLERANCE);
    if (item.Similarity(bluecandle) > CAPTURED_SHOP_ITEM_SIMILARITY_THRESHOLD) {
      ZeldaInformationHandler::SetSecret(mapx, mapy, ZeldaInformationHandler::Secrets::CANDLE_SHOP);
      foundSecret = true;
    }
    if (item.Similarity(bomb) > CAPTURED_SHOP_ITEM_SIMILARITY_THRESHOLD) {
      ZeldaInformationHandler::SetSecret(mapx, mapy, ZeldaInformationHandler::Secrets::BOMB_SHOP);
      foundSecret = true;
    }
    if (item.Similarity(arrow) > CAPTURED_SHOP_ITEM_SIMILARITY_THRESHOLD) {
      ZeldaInformationHandler::SetSecret(mapx, mapy, ZeldaInformationHandler::Secrets::ARROW_SHOP);
      foundSecret = true;
    }
    if (item.Similarity(bait) > CAPTURED_SHOP_ITEM_SIMILARITY_THRESHOLD) {
      ZeldaInformationHandler::SetSecret(mapx, mapy, ZeldaInformationHandler::Secrets::BAIT_SHOP);
      foundSecret = true;
    }
    if (item.Similarity(bluering) > CAPTURED_SHOP_ITEM_SIMILARITY_THRESHOLD) {
      ZeldaInformationHandler::SetSecret(mapx, mapy, ZeldaInformationHandler::Secrets::BLUE_RING_SHOP);
      foundSecret = true;
    }
    if (item.Similarity(woodsword) > CAPTURED_SHOP_ITEM_SIMILARITY_THRESHOLD) {
      ZeldaInformationHandler::SetSecret(mapx, mapy, ZeldaInformationHandler::Secrets::WOOD_SWORD);
      foundSecret = true;
    }
  }
  {
    ImageHandler item = screen.Crop(REFERENCE_SHOP_RIGHT_ITEM_XCOOR*SCALE_X, REFERENCE_SHOP_RIGHT_ITEM_YCOOR*SCALE_Y, REFERENCE_SHOP_RIGHT_ITEM_WIDTH*SCALE_X, REFERENCE_SHOP_RIGHT_ITEM_HEIGHT*SCALE_Y).FilterRGB(BLACK_R, BLACK_G, BLACK_B, CAPTURED_ITEM_COLOR_TOLERANCE);
    if (item.Similarity(bluecandle) > CAPTURED_SHOP_ITEM_SIMILARITY_THRESHOLD) {
      ZeldaInformationHandler::SetSecret(mapx, mapy, ZeldaInformationHandler::Secrets::CANDLE_SHOP);
      foundSecret = true;
    }
    if (item.Similarity(bomb) > CAPTURED_SHOP_ITEM_SIMILARITY_THRESHOLD) {
      ZeldaInformationHandler::SetSecret(mapx, mapy, ZeldaInformationHandler::Secrets::BOMB_SHOP);
      foundSecret = true;
    }
    if (item.Similarity(arrow) > CAPTURED_SHOP_ITEM_SIMILARITY_THRESHOLD) {
      ZeldaInformationHandler::SetSecret(mapx, mapy, ZeldaInformationHandler::Secrets::ARROW_SHOP);
      foundSecret = true;
    }
    if (item.Similarity(bait) > CAPTURED_SHOP_ITEM_SIMILARITY_THRESHOLD) {
      ZeldaInformationHandler::SetSecret(mapx, mapy, ZeldaInformationHandler::Secrets::BAIT_SHOP);
      foundSecret = true;
    }
    if (item.Similarity(bluering) > CAPTURED_SHOP_ITEM_SIMILARITY_THRESHOLD) {
      ZeldaInformationHandler::SetSecret(mapx, mapy, ZeldaInformationHandler::Secrets::BLUE_RING_SHOP);
      foundSecret = true;
    }
    if (item.Similarity(woodsword) > CAPTURED_SHOP_ITEM_SIMILARITY_THRESHOLD) {
      ZeldaInformationHandler::SetSecret(mapx, mapy, ZeldaInformationHandler::Secrets::WOOD_SWORD);
      foundSecret = true;
    }
    if (item.Similarity(potion) > CAPTURED_SHOP_ITEM_SIMILARITY_THRESHOLD) {
      rpotionshop = true;
    }
  }
  //Distinguish potion shop from bonus caves
  if (lpotionshop && rpotionshop) {
    ZeldaInformationHandler::SetSecret(mapx, mapy, ZeldaInformationHandler::Secrets::POTION_SHOP);
    foundSecret = true;
  }
  if (lpotionshop && !rpotionshop) {
    ZeldaInformationHandler::SetSecret(mapx, mapy, ZeldaInformationHandler::Secrets::BONUS_CAVE);
    foundSecret = true;		  
  }
  if (!foundSecret) {
    ZeldaInformationHandler::SetSecret(mapx, mapy, ZeldaInformationHandler::Secrets::UNKNOWN_CAVE);
  }
}

void ZeldaImageProcessor::InitMinimap(ImageHandler& screen) {
  int numhearts = 0;
  //check for hearts
  for (int xsep = 0; xsep < 8; ++xsep) {
    for (int ysep = 0; ysep < 2; ++ysep) {
      int xcoor = REFERENCE_HEART_XCOOR + xsep * REFERENCE_HEART_X_SEPERATION;
      int ycoor = REFERENCE_HEART_YCOOR + ysep * REFERENCE_HEART_Y_SEPERATION;
      ImageHandler heartloc = screen.Crop(xcoor*SCALE_X, ycoor*SCALE_Y, REFERENCE_HEART_WIDTH*SCALE_X, REFERENCE_HEART_HEIGHT*SCALE_Y).FilterRGB(BLACK_R, BLACK_G, BLACK_B, CAPTURED_HEART_COLOR_TOLERANCE);
      if (heartloc.Similarity(heart) > CAPTURED_HEART_SIMILARITY_THRESHOLD) {
	numhearts++;
      }
    }
  }
  if (numhearts >= 3) {
    ImageHandler minimap = screen.Crop(REFERENCE_OVERWORLD_MINIMAP_XCOOR*SCALE_X, REFERENCE_OVERWORLD_MINIMAP_YCOOR*SCALE_Y, REFERENCE_OVERWORLD_MINIMAP_WIDTH*SCALE_X, REFERENCE_OVERWORLD_MINIMAP_HEIGHT*SCALE_Y);
    std::tuple<int, int, int> rgb = minimap.MostCommonRGB();
    int r = std::get<0>(rgb);
    int g = std::get<1>(rgb);
    int b = std::get<2>(rgb);
    if (!(r == BLACK_R && g == BLACK_G && b == BLACK_B)) {
      MINIMAP_GRAY_R = r;
      MINIMAP_GRAY_G = g;
      MINIMAP_GRAY_B = b;
      INITIALIZED_MINIMAP_GRAY = true;
    }
  }
}

void ZeldaImageProcessor::InitTunic(ImageHandler& screen) {
  ImageHandler minimap = screen.Crop(REFERENCE_OVERWORLD_MINIMAP_XCOOR*SCALE_X, REFERENCE_OVERWORLD_MINIMAP_YCOOR*SCALE_Y, REFERENCE_OVERWORLD_MINIMAP_WIDTH*SCALE_X, REFERENCE_OVERWORLD_MINIMAP_HEIGHT*SCALE_Y);
  std::tuple<int, int, int> rgb = minimap.MostCommonRGB();
  std::tuple<int, int, int> minimapGray = std::make_tuple(MINIMAP_GRAY_R, MINIMAP_GRAY_G, MINIMAP_GRAY_B);
  if (rgb == minimapGray) {
    //check every minimap spot
    for (int mapx = 0; mapx < 16; ++mapx) {
      for (int mapy = 0; mapy < 8; ++mapy) {
	int tx = (static_cast<double>(mapx) / 16) * REFERENCE_OVERWORLD_MINIMAP_WIDTH * SCALE_X;
	int ty = (static_cast<double>(mapy) / 8) * REFERENCE_OVERWORLD_MINIMAP_HEIGHT * SCALE_Y;
	int tw = REFERENCE_OVERWORLD_MINIMAP_WIDTH * SCALE_X / 16;
	int th = REFERENCE_OVERWORLD_MINIMAP_HEIGHT * SCALE_Y / 8;
	int sx = REFERENCE_OVERWORLD_MINIMAP_CURSOR_XCOOR * SCALE_X;
	int sy = REFERENCE_OVERWORLD_MINIMAP_CURSOR_YCOOR * SCALE_Y;
	int sw = REFERENCE_OVERWORLD_MINIMAP_CURSOR_WIDTH * SCALE_X;
	int sh = REFERENCE_OVERWORLD_MINIMAP_CURSOR_HEIGHT * SCALE_Y;	  
	ImageHandler mapspot = minimap.Crop(tx, ty, tw, th).Crop(sx, sy, sw, sh);
	std::tuple<int, int, int> maprgb = mapspot.MostCommonRGB();
	if (maprgb != std::make_tuple(BLACK_R, BLACK_G, BLACK_B) && maprgb != minimapGray) {
	  TUNIC_GREEN_R = std::get<0>(maprgb);
	  TUNIC_GREEN_G = std::get<1>(maprgb);
	  TUNIC_GREEN_B = std::get<2>(maprgb);
	  CURRENT_TUNIC_R = TUNIC_GREEN_R;
	  CURRENT_TUNIC_G = TUNIC_GREEN_G;
	  CURRENT_TUNIC_B = TUNIC_GREEN_B;
	  {
	    std::tuple<int, int, int> heartrgb = screen.Crop(REFERENCE_HEART_RED_XCOOR*SCALE_X, REFERENCE_HEART_RED_YCOOR*SCALE_Y, REFERENCE_HEART_RED_WIDTH*SCALE_X, REFERENCE_HEART_RED_HEIGHT*SCALE_Y).MostCommonRGB();
	    HEART_RED_R = std::get<0>(heartrgb);
	    HEART_RED_G = std::get<1>(heartrgb);
	    HEART_RED_B = std::get<2>(heartrgb);
	  }
	  {
	    std::tuple<int, int, int> heartrgb = screen.Crop(REFERENCE_START_BLUE_XCOOR*SCALE_X, REFERENCE_START_BLUE_YCOOR*SCALE_Y, REFERENCE_START_BLUE_WIDTH*SCALE_X, REFERENCE_START_BLUE_HEIGHT*SCALE_Y).MostCommonRGB();
	    START_BLUE_R = std::get<0>(heartrgb);
	    START_BLUE_G = std::get<1>(heartrgb);
	    START_BLUE_B = std::get<2>(heartrgb);
	  }
	  INITIALIZED_TUNIC_GREEN = true;
	  INITIALIZED_CURRENT_TUNIC = true;
	}
      }
    }
  }
}

int ZeldaImageProcessor::Hearts(ImageHandler& screen, bool empty) {
  int numhearts = 0;
  for (int xsep = 0; xsep < 8; ++xsep) {
    for (int ysep = 0; ysep < 2; ++ysep) {
      int xcoor = REFERENCE_HEART_XCOOR + xsep * REFERENCE_HEART_X_SEPERATION;
      int ycoor = REFERENCE_HEART_YCOOR + ysep * REFERENCE_HEART_Y_SEPERATION;
      ImageHandler heartloc = screen.Crop(xcoor*SCALE_X, ycoor*SCALE_Y, REFERENCE_HEART_WIDTH*SCALE_X, REFERENCE_HEART_HEIGHT*SCALE_Y);
      if (empty) {
	if (heartloc.Similarity(heart) > CAPTURED_HEART_SIMILARITY_THRESHOLD) {
	  numhearts++;
	}	
      }
      else {
	double rp = static_cast<double>(heartloc.PixelsWithRGB(HEART_RED_R, HEART_RED_G, HEART_RED_B).size()) / (heartloc.Width() * heartloc.Height());
	if (rp > CAPTURED_FULL_HEART_COLOR_THRESHOLD/2) {
	  numhearts++;
	}
      }
    }
  }
  return numhearts;
}

void ZeldaImageProcessor::RecordDungeonItems(ImageHandler& screen) {
  //The minimap cursor does not show in staircases
  //check if alive  
  if (Hearts(screen, false) > 0) {
    double maxSim = 0;
    Dungeon::DungeonItems type;
    ImageHandler item = screen.Crop(REFERENCE_DUNGEON_ITEM_XCOOR*SCALE_X, REFERENCE_DUNGEON_ITEM_YCOOR*SCALE_Y, REFERENCE_DUNGEON_ITEM_WIDTH*SCALE_X, REFERENCE_DUNGEON_ITEM_HEIGHT*SCALE_Y).FilterRGB(BLACK_R, BLACK_G, BLACK_B, CAPTURED_DUNGEON_ITEM_COLOR_TOLERANCE);
    if (item.Similarity(book) > maxSim) {
      maxSim = item.Similarity(book);
      type = Dungeon::DungeonItems::BOOK;
    }
    if (item.Similarity(bow) > maxSim) {
      maxSim = item.Similarity(bow);
      type = Dungeon::DungeonItems::BOW;
    }
    if (item.Similarity(heartcontainer) > maxSim) {
      maxSim = item.Similarity(heartcontainer);
      type = Dungeon::DungeonItems::HEART_CONTAINER;
    }	      
    if (item.Similarity(ladder) > maxSim) {
      maxSim = item.Similarity(ladder);
      type = Dungeon::DungeonItems::LADDER;
    }
    if (item.Similarity(magicalkey) > maxSim) {
      maxSim = item.Similarity(magicalkey);
      type = Dungeon::DungeonItems::MAGICAL_KEY;
    }
    if (item.Similarity(woodenboomerang) > maxSim) {
      maxSim = item.Similarity(woodenboomerang);
      type = Dungeon::DungeonItems::WOODEN_BOOMERANG;
    }	      
    if (item.Similarity(magicalboomerang) > maxSim) {
      maxSim = item.Similarity(magicalboomerang);
      type = Dungeon::DungeonItems::MAGICAL_BOOMERANG;
    }	      
    if (item.Similarity(powerbracelet) > maxSim) {
      maxSim = item.Similarity(powerbracelet);
      type = Dungeon::DungeonItems::POWER_BRACELET;
    }	      
    if (item.Similarity(raft) > maxSim) {
      maxSim = item.Similarity(raft);
      type = Dungeon::DungeonItems::RAFT;
    }
    if (item.Similarity(recorder) > maxSim) {
      maxSim = item.Similarity(recorder);
      type = Dungeon::DungeonItems::RECORDER;
    }
    if (item.Similarity(redcandle) > maxSim) {
      maxSim = item.Similarity(redcandle);
      type = Dungeon::DungeonItems::RED_CANDLE;
    }
    if (item.Similarity(redring) > maxSim) {
      maxSim = item.Similarity(redring);
      type = Dungeon::DungeonItems::RED_RING;
    }
    if (item.Similarity(silverarrow) > maxSim) {
      maxSim = item.Similarity(silverarrow);
      type = Dungeon::DungeonItems::SILVER_ARROW;
    }
    if (item.Similarity(wand) > maxSim) {
      maxSim = item.Similarity(wand);
      type = Dungeon::DungeonItems::WAND;
    }	      
    if (item.Similarity(whitesworditem) > maxSim) {
      maxSim = item.Similarity(whitesworditem);
      type = Dungeon::DungeonItems::WHITE_SWORD;
    }	      
    //prevent false positive
    double bp = static_cast<double>(item.PixelsWithRGB(BLACK_R, BLACK_G, BLACK_B).size()) / (item.Width() * item.Height());
    if (bp > maxSim) {
      maxSim = 0;
      type = Dungeon::DungeonItems::NONE;
    }
    if ((1-bp) > maxSim) {
      maxSim = 0;
      type = Dungeon::DungeonItems::NONE;
    }	      
    if (maxSim > CAPTURED_DUNGEON_ITEM_SIMILARITY_THRESHOLD) {
      ZeldaInformationHandler::SetItem(type);
    }
  }
}

void ZeldaImageProcessor::CheckWhiteSwordCaveItem(ImageHandler& screen, int mapx, int mapy) {
  //check if alive  
  if (Hearts(screen, false) > 0) {
    double maxSim = 0;
    Dungeon::DungeonItems type;
    ImageHandler item = screen.Crop(REFERENCE_WHITE_SWORD_CAVE_ITEM_XCOOR*SCALE_X, REFERENCE_WHITE_SWORD_CAVE_ITEM_YCOOR*SCALE_Y, REFERENCE_WHITE_SWORD_CAVE_ITEM_WIDTH*SCALE_X, REFERENCE_WHITE_SWORD_CAVE_ITEM_HEIGHT*SCALE_Y).FilterRGB(BLACK_R, BLACK_G, BLACK_B, CAPTURED_WHITE_SWORD_CAVE_ITEM_COLOR_TOLERANCE);
    if (item.Similarity(book) > maxSim) {
      maxSim = item.Similarity(book);
      type = Dungeon::DungeonItems::BOOK;
    }
    if (item.Similarity(bow) > maxSim) {
      maxSim = item.Similarity(bow);
      type = Dungeon::DungeonItems::BOW;
    }
    if (item.Similarity(heartcontainer) > maxSim) {
      maxSim = item.Similarity(heartcontainer);
      type = Dungeon::DungeonItems::HEART_CONTAINER;
    }
    if (item.Similarity(ladder) > maxSim) {
      maxSim = item.Similarity(ladder);
      type = Dungeon::DungeonItems::LADDER;
    }
    if (item.Similarity(magicalkey) > maxSim) {
      maxSim = item.Similarity(magicalkey);
      type = Dungeon::DungeonItems::MAGICAL_KEY;
    }
    if (item.Similarity(woodenboomerang) > maxSim) {
      maxSim = item.Similarity(woodenboomerang);
      type = Dungeon::DungeonItems::WOODEN_BOOMERANG;
    }
    if (item.Similarity(magicalboomerang) > maxSim) {
      maxSim = item.Similarity(magicalboomerang);
      type = Dungeon::DungeonItems::MAGICAL_BOOMERANG;
    }
    if (item.Similarity(powerbracelet) > maxSim) {
      maxSim = item.Similarity(powerbracelet);
      type = Dungeon::DungeonItems::POWER_BRACELET;
    }
    if (item.Similarity(raft) > maxSim) {
      maxSim = item.Similarity(raft);
      type = Dungeon::DungeonItems::RAFT;
    }
    if (item.Similarity(recorder) > maxSim) {
      maxSim = item.Similarity(recorder);
      type = Dungeon::DungeonItems::RECORDER;
    }
    if (item.Similarity(redcandle) > maxSim) {
      maxSim = item.Similarity(redcandle);
      type = Dungeon::DungeonItems::RED_CANDLE;
    }
    if (item.Similarity(redring) > maxSim) {
      maxSim = item.Similarity(redring);
      type = Dungeon::DungeonItems::RED_RING;
    }
    if (item.Similarity(silverarrow) > maxSim) {
      maxSim = item.Similarity(silverarrow);
      type = Dungeon::DungeonItems::SILVER_ARROW;
    }
    if (item.Similarity(wand) > maxSim) {
      maxSim = item.Similarity(wand);
      type = Dungeon::DungeonItems::WAND;
    }
    if (item.Similarity(whitesworditem) > maxSim) {
      maxSim = item.Similarity(whitesworditem);
      type = Dungeon::DungeonItems::WHITE_SWORD;
    }
    //prevent false positive
    double bp = static_cast<double>(item.PixelsWithRGB(BLACK_R, BLACK_G, BLACK_B).size()) / (item.Width() * item.Height());
    if (bp > maxSim) {
      maxSim = 0;
      type = Dungeon::DungeonItems::NONE;
    }
    if ((1 - bp) > maxSim) {
      maxSim = 0;
      type = Dungeon::DungeonItems::NONE;
    }
    if (maxSim > CAPTURED_WHITE_SWORD_CAVE_ITEM_SIMILARITY_THRESHOLD) {
      ZeldaInformationHandler::SetSecret(mapx, mapy, ZeldaInformationHandler::Secrets::WHITE_SWORD);
    }
  }
}

void ZeldaImageProcessor::CheckDungeonRing(ImageHandler& minimap) {
  int possibles = 0;
  std::tuple<int, int, int> color;
  for (int mapx = 0; mapx < 8; ++mapx) {
    for (int mapy = 0; mapy < 8; ++mapy) {
      if (ZeldaInformationHandler::GetDungeonRoomType(mapx, mapy) != Dungeon::RoomType::TRIFORCE_ROOM) {
	int tx = (static_cast<double>(mapx) / 8) * REFERENCE_OVERWORLD_MINIMAP_WIDTH * SCALE_X;
	int ty = (static_cast<double>(mapy) / 8) * REFERENCE_OVERWORLD_MINIMAP_HEIGHT * SCALE_Y;
	int tw = REFERENCE_OVERWORLD_MINIMAP_WIDTH * SCALE_X / 8;
	int th = REFERENCE_OVERWORLD_MINIMAP_HEIGHT * SCALE_Y / 8;
	int sx = REFERENCE_DUNGEON_MINIMAP_CURSOR_XCOOR * SCALE_X;
	int sy = REFERENCE_DUNGEON_MINIMAP_CURSOR_YCOOR * SCALE_Y;
	int sw = REFERENCE_DUNGEON_MINIMAP_CURSOR_WIDTH * SCALE_X;
	int sh = REFERENCE_DUNGEON_MINIMAP_CURSOR_HEIGHT * SCALE_Y;
	ImageHandler mapspot = minimap.Crop(tx, ty, tw, th);
	ImageHandler mapbox = mapspot.Crop(sx, sy, sw, sh);
	std::tuple<int, int, int> maprgb = mapspot.MostCommonRGB();
	if (maprgb != std::make_tuple(BLACK_R, BLACK_G, BLACK_B)) {
	  mapspot = mapspot.FilterRGB(std::get<0>(maprgb), std::get<1>(maprgb), std::get<2>(maprgb), CAPTURED_DUNGEON_MINIMAP_CURSOR_COLOR_TOLERANCE);
	  double b = static_cast<double>(mapspot.PixelsWithRGB(BLACK_R, BLACK_G, BLACK_B).size());
	  mapbox = mapbox.FilterRGB(std::get<0>(maprgb), std::get<1>(maprgb), std::get<2>(maprgb), CAPTURED_DUNGEON_MINIMAP_CURSOR_COLOR_TOLERANCE);
	  double mb = static_cast<double>(mapbox.PixelsWithRGB(BLACK_R, BLACK_G, BLACK_B).size());
	  double bp = b / (mapspot.Width() * mapspot.Height());
	  double mbp = (mb-b) / (mapbox.Width() * mapbox.Height());		    
	  if (bp > CAPTURED_DUNGEON_MINIMAP_CURSOR_UPPER_BLACK_THRESHOLD && mbp < CAPTURED_DUNGEON_MINIMAP_CURSOR_LOWER_BLACK_THRESHOLD) {
	    possibles++;
	    color = maprgb;
	  }		  		    
	}
      }
    }
  }
  if (possibles == 1 && color == std::make_tuple(HEART_RED_R, HEART_RED_G, HEART_RED_B)) {
    CURRENT_TUNIC_R = std::get<0>(color);
    CURRENT_TUNIC_G = std::get<1>(color);
    CURRENT_TUNIC_B = std::get<2>(color);
  }
}

void ZeldaImageProcessor::CheckTriforce(ImageHandler& screen) {
  if (Hearts(screen, false) >= ZeldaInformationHandler::GetHearts()) {
    //if full health, then check for link holding triforce over a black screen
    ImageHandler playScreen = screen.Crop(REFERENCE_PLAYING_SCREEN_XCOOR*SCALE_X, REFERENCE_PLAYING_SCREEN_YCOOR*SCALE_Y, REFERENCE_PLAYING_SCREEN_WIDTH*SCALE_X, REFERENCE_PLAYING_SCREEN_HEIGHT*SCALE_Y).FilterRGB(BLACK_R, BLACK_G, BLACK_B);
    //remove edges
    playScreen = playScreen.Crop(2*SCALE_X, 2*SCALE_Y, playScreen.Width()-4*SCALE_X, playScreen.Height()-4*SCALE_Y);
    double bp = static_cast<double>(playScreen.PixelsWithRGB(BLACK_R, BLACK_G, BLACK_B).size()) / (playScreen.Width() * playScreen.Height());
    if (bp > DUNGEON_TRIFORCE_BLACK_THRESHOLD && bp < 1) {
      std::vector<int> box = BoundingBox(playScreen.PixelsWithRGB(WHITE_R, WHITE_G, WHITE_B));
      if (box[2]/SCALE_X < DUNGEON_TRIFORCE_WIDTH_UPPER_THRESHOLD && box[3]/SCALE_Y < DUNGEON_TRIFORCE_HEIGHT_UPPER_THRESHOLD &&
	  box[2]/SCALE_X > DUNGEON_TRIFORCE_WIDTH_LOWER_THRESHOLD && box[3]/SCALE_Y > DUNGEON_TRIFORCE_HEIGHT_LOWER_THRESHOLD) {
	ZeldaInformationHandler::SetTriforce();
      }
    }
  }
}

void ZeldaImageProcessor::RecordStaircase(ImageHandler& screen) {
  ImageHandler playScreen = screen.Crop(REFERENCE_PLAYING_SCREEN_XCOOR*SCALE_X, REFERENCE_PLAYING_SCREEN_YCOOR*SCALE_Y, REFERENCE_PLAYING_SCREEN_WIDTH*SCALE_X, REFERENCE_PLAYING_SCREEN_HEIGHT*SCALE_Y).FilterRGB(BLACK_R, BLACK_G, BLACK_B, CAPTURED_DUNGEON_STAIRCASE_COLOR_TOLERANCE);
  if (playScreen.Similarity(stairway) > CAPTURED_DUNGEON_STAIRCASE_THRESHOLD) {
    ZeldaInformationHandler::SetStaircase();
  }
}

void ZeldaImageProcessor::RecordDoors(ImageHandler& screen, int lmapx, int lmapy) {
  //first make sure you're not moving between doors
  std::pair<int, int> prevloc = ZeldaInformationHandler::GetDungeonLocation();
  if (prevloc != std::make_pair(lmapx, lmapy)) {
    dungeondoortransition = false;
  }
  ImageHandler handle = screen.Crop(REFERENCE_DUNGEON_DOOR_HANDLE_XCOOR*SCALE_X, REFERENCE_DUNGEON_DOOR_HANDLE_YCOOR*SCALE_Y, REFERENCE_DUNGEON_DOOR_HANDLE_WIDTH*SCALE_X, REFERENCE_DUNGEON_DOOR_HANDLE_HEIGHT*SCALE_Y);
  std::tuple<int, int, int> hcolor = handle.MostCommonRGB();
  double bp = handle.PixelsWithRGB(BLACK_R, BLACK_G, BLACK_B).size() / handle.Width() / handle.Height();
  handle = handle.FilterRGB(std::get<0>(hcolor), std::get<1>(hcolor), std::get<2>(hcolor), CAPTURED_DUNGEON_DOOR_HANDLE_COLOR_TOLERANCE);
  double sim =  std::max(handle.Similarity(dungeondoorhandle), handle.Similarity(dungeondoorhandleblack));
  if (sim > CAPTURED_DUNGEON_DOOR_HANDLE_SIMILARITY_THRESHOLD || 1-sim > CAPTURED_DUNGEON_DOOR_HANDLE_SIMILARITY_THRESHOLD) {
    if (!dungeondoortransition) {
      RecordNormalDoors(screen, lmapx, lmapy);
      RecordDarkDoors(screen, lmapx, lmapy);
      RecordBlackDoors(screen, lmapx, lmapy);
    }
  }
  else {
    //case where you walk into a black room
    if (bp < 1) {
      dungeondoortransition = true;
    }
  }
}

void ZeldaImageProcessor::CheckCompass(ImageHandler& minimap, int lmapx, int lmapy) {
  for (int mapx = 0; mapx < 8; ++mapx) {
    for (int mapy = 0; mapy < 8; ++mapy) {
      int tx = (static_cast<double>(mapx) / 8) * REFERENCE_OVERWORLD_MINIMAP_WIDTH * SCALE_X;
      int ty = (static_cast<double>(mapy) / 8) * REFERENCE_OVERWORLD_MINIMAP_HEIGHT * SCALE_Y;
      int tw = REFERENCE_OVERWORLD_MINIMAP_WIDTH * SCALE_X / 8;
      int th = REFERENCE_OVERWORLD_MINIMAP_HEIGHT * SCALE_Y / 8;
      int sx = REFERENCE_DUNGEON_MINIMAP_CURSOR_XCOOR * SCALE_X;
      int sy = REFERENCE_DUNGEON_MINIMAP_CURSOR_YCOOR * SCALE_Y;
      int sw = REFERENCE_DUNGEON_MINIMAP_CURSOR_WIDTH * SCALE_X;
      int sh = REFERENCE_DUNGEON_MINIMAP_CURSOR_HEIGHT * SCALE_Y;
      ImageHandler mapbox = minimap.Crop(tx, ty, tw, th);
      ImageHandler mapspot = minimap.Crop(tx, ty, tw, th).Crop(sx, sy, sw, sh);
      std::tuple<int, int, int> maprgb = mapspot.MostCommonRGB();
      if (maprgb != std::make_tuple(START_BLUE_R, START_BLUE_G, START_BLUE_B) && maprgb != std::make_tuple(BLACK_R, BLACK_G, BLACK_B) && !(mapx == lmapx && mapy == lmapy)) {
	mapspot = mapspot.FilterRGB(std::get<0>(maprgb), std::get<1>(maprgb), std::get<2>(maprgb), CAPTURED_DUNGEON_MINIMAP_CURSOR_COLOR_TOLERANCE);
	double b = static_cast<double>(mapspot.PixelsWithRGB(BLACK_R, BLACK_G, BLACK_B).size());
	mapbox = mapbox.FilterRGB(std::get<0>(maprgb), std::get<1>(maprgb), std::get<2>(maprgb), CAPTURED_DUNGEON_MINIMAP_CURSOR_COLOR_TOLERANCE);
	double mb = static_cast<double>(mapbox.PixelsWithRGB(BLACK_R, BLACK_G, BLACK_B).size());
	double bp = b / (mapspot.Width() * mapspot.Height());
	double mbp = (mb-b) / (mapbox.Width() * mapbox.Height());		    
	if (bp > CAPTURED_DUNGEON_MINIMAP_CURSOR_UPPER_BLACK_THRESHOLD && mbp < CAPTURED_DUNGEON_MINIMAP_CURSOR_LOWER_BLACK_THRESHOLD) {
	  ZeldaInformationHandler::SetDungeonLocation(mapx, mapy, Dungeon::RoomType::TRIFORCE_ROOM);
	}
      }
    }
  }
}

void ZeldaImageProcessor::RecordDungeonMinimap(ImageHandler& minimap) {
  for (int mapx = 0; mapx < 8; ++mapx) {
    for (int mapy = 0; mapy < 8; ++mapy) {
      int tx = (static_cast<double>(mapx) / 8) * REFERENCE_OVERWORLD_MINIMAP_WIDTH * SCALE_X;
      int ty = (static_cast<double>(mapy) / 8) * REFERENCE_OVERWORLD_MINIMAP_HEIGHT * SCALE_Y;
      int tw = REFERENCE_OVERWORLD_MINIMAP_WIDTH * SCALE_X / 8;
      int th = REFERENCE_OVERWORLD_MINIMAP_HEIGHT * SCALE_Y / 8;
      int sx = REFERENCE_DUNGEON_MINIMAP_CURSOR_XCOOR * SCALE_X;
      int sy = REFERENCE_DUNGEON_MINIMAP_CURSOR_YCOOR * SCALE_Y;
      int sw = REFERENCE_DUNGEON_MINIMAP_CURSOR_WIDTH * SCALE_X;
      int sh = REFERENCE_DUNGEON_MINIMAP_CURSOR_HEIGHT * SCALE_Y;
      ImageHandler mapspot = minimap.Crop(tx, ty, tw, th).Crop(sx, sy, sw, sh);
      std::tuple<int, int, int> maprgb = mapspot.MostCommonRGB();
      if (maprgb == std::make_tuple(START_BLUE_R, START_BLUE_G, START_BLUE_B)) {
	ZeldaInformationHandler::SetDungeonLocation(mapx, mapy, Dungeon::RoomType::UNSEEN_ROOM);
      }
    }
  }
}

void ZeldaImageProcessor::LoadTextures() {
  arrow = ImageHandler::LoadPNG("Images/Shops/Arrow.png").FilterRGB(BLACK_R, BLACK_G, BLACK_B);
  bait = ImageHandler::LoadPNG("Images/Shops/Bait.png").FilterRGB(BLACK_R, BLACK_G, BLACK_B);
  bomb = ImageHandler::LoadPNG("Images/Shops/Bomb.png").FilterRGB(BLACK_R, BLACK_G, BLACK_B);  
  bluering =ImageHandler::LoadPNG("Images/Shops/BlueRing.png").FilterRGB(BLACK_R, BLACK_G, BLACK_B);
  bluecandle = ImageHandler::LoadPNG("Images/Shops/BlueCandle.png").FilterRGB(BLACK_R, BLACK_G, BLACK_B);
  potion = ImageHandler::LoadPNG("Images/Shops/Potion.png").FilterRGB(BLACK_R, BLACK_G, BLACK_B);
  woodsword = ImageHandler::LoadPNG("Images/Shops/WoodSword.png").FilterRGB(BLACK_R, BLACK_G, BLACK_B);  
  whitesword = ImageHandler::LoadPNG("Images/Shops/WhiteSword.png").FilterRGB(WHITE_R, WHITE_G, WHITE_B);
  magicalsword = ImageHandler::LoadPNG("Images/Shops/MagicalSword.png").FilterRGB(WHITE_R, WHITE_G, WHITE_B);
  anyroad = ImageHandler::LoadPNG("Images/Shops/Anyroad.png").FilterRGB(WHITE_R, WHITE_G, WHITE_B);
  dungeonnine = ImageHandler::LoadPNG("Images/Dungeon/DungeonNine.png");
  dungeoneight = ImageHandler::LoadPNG("Images/Dungeon/DungeonEight.png");
  dungeonseven = ImageHandler::LoadPNG("Images/Dungeon/DungeonSeven.png");
  dungeonsix = ImageHandler::LoadPNG("Images/Dungeon/DungeonSix.png");
  dungeonfive = ImageHandler::LoadPNG("Images/Dungeon/DungeonFive.png");
  dungeonfour = ImageHandler::LoadPNG("Images/Dungeon/DungeonFour.png");
  dungeonthree = ImageHandler::LoadPNG("Images/Dungeon/DungeonThree.png");
  dungeontwo = ImageHandler::LoadPNG("Images/Dungeon/DungeonTwo.png");
  dungeonone = ImageHandler::LoadPNG("Images/Dungeon/DungeonOne.png");  
  dungeonld = ImageHandler::LoadPNG("Images/Dungeon/DungeonLeftDoor.png").FilterRGB(BLACK_R, BLACK_G, BLACK_B);
  dungeonlk = ImageHandler::LoadPNG("Images/Dungeon/LeftKeyDoor.png");
  dungeonls = ImageHandler::LoadPNG("Images/Dungeon/LeftShutterDoor.png");
  dungeonlb = ImageHandler::LoadPNG("Images/Dungeon/LeftBombDoor.png").FilterRGB(BLACK_R, BLACK_G, BLACK_B);
  dungeonrd = ImageHandler::LoadPNG("Images/Dungeon/DungeonRightDoor.png").FilterRGB(BLACK_R, BLACK_G, BLACK_B);
  dungeonrk = ImageHandler::LoadPNG("Images/Dungeon/RightKeyDoor.png");
  dungeonrs = ImageHandler::LoadPNG("Images/Dungeon/RightShutterDoor.png");
  dungeonrb = ImageHandler::LoadPNG("Images/Dungeon/RightBombDoor.png").FilterRGB(BLACK_R, BLACK_G, BLACK_B);
  dungeontd = ImageHandler::LoadPNG("Images/Dungeon/DungeonTopDoor.png").FilterRGB(BLACK_R, BLACK_G, BLACK_B);
  dungeontk = ImageHandler::LoadPNG("Images/Dungeon/TopKeyDoor.png");
  dungeonts = ImageHandler::LoadPNG("Images/Dungeon/TopShutterDoor.png");
  dungeontb = ImageHandler::LoadPNG("Images/Dungeon/TopBombDoor.png").FilterRGB(BLACK_R, BLACK_G, BLACK_B);
  dungeonbd = ImageHandler::LoadPNG("Images/Dungeon/DungeonBottomDoor.png").FilterRGB(BLACK_R, BLACK_G, BLACK_B);
  dungeonbk = ImageHandler::LoadPNG("Images/Dungeon/BottomKeyDoor.png");
  dungeonbs = ImageHandler::LoadPNG("Images/Dungeon/BottomShutterDoor.png");
  dungeonbb = ImageHandler::LoadPNG("Images/Dungeon/BottomBombDoor.png").FilterRGB(BLACK_R, BLACK_G, BLACK_B);
  dungeondld = ImageHandler::LoadPNG("Images/Dungeon/DungeonLeftDoorDark.png").FilterRGB(BLACK_R, BLACK_G, BLACK_B);
  dungeondlk = ImageHandler::LoadPNG("Images/Dungeon/LeftKeyDoorDark.png").FilterRGB(BLACK_R, BLACK_G, BLACK_B);
  dungeondls = ImageHandler::LoadPNG("Images/Dungeon/LeftShutterDoorDark.png").FilterRGB(BLACK_R, BLACK_G, BLACK_B);
  dungeondlb = ImageHandler::LoadPNG("Images/Dungeon/LeftBombDoorDark.png").FilterRGB(BLACK_R, BLACK_G, BLACK_B);
  dungeondrd = ImageHandler::LoadPNG("Images/Dungeon/DungeonRightDoorDark.png").FilterRGB(BLACK_R, BLACK_G, BLACK_B);
  dungeondrk = ImageHandler::LoadPNG("Images/Dungeon/RightKeyDoorDark.png").FilterRGB(BLACK_R, BLACK_G, BLACK_B);
  dungeondrs = ImageHandler::LoadPNG("Images/Dungeon/RightShutterDoorDark.png").FilterRGB(BLACK_R, BLACK_G, BLACK_B);
  dungeondrb = ImageHandler::LoadPNG("Images/Dungeon/RightBombDoorDark.png").FilterRGB(BLACK_R, BLACK_G, BLACK_B);
  dungeondtd = ImageHandler::LoadPNG("Images/Dungeon/DungeonTopDoorDark.png").FilterRGB(BLACK_R, BLACK_G, BLACK_B);
  dungeondtk = ImageHandler::LoadPNG("Images/Dungeon/TopKeyDoorDark.png").FilterRGB(BLACK_R, BLACK_G, BLACK_B);
  dungeondts = ImageHandler::LoadPNG("Images/Dungeon/TopShutterDoorDark.png").FilterRGB(BLACK_R, BLACK_G, BLACK_B);
  dungeondtb = ImageHandler::LoadPNG("Images/Dungeon/TopBombDoorDark.png").FilterRGB(BLACK_R, BLACK_G, BLACK_B);
  dungeondbd = ImageHandler::LoadPNG("Images/Dungeon/DungeonBottomDoorDark.png").FilterRGB(BLACK_R, BLACK_G, BLACK_B);
  dungeondbk = ImageHandler::LoadPNG("Images/Dungeon/BottomKeyDoorDark.png").FilterRGB(BLACK_R, BLACK_G, BLACK_B);
  dungeondbs = ImageHandler::LoadPNG("Images/Dungeon/BottomShutterDoorDark.png").FilterRGB(BLACK_R, BLACK_G, BLACK_B);
  dungeondbb = ImageHandler::LoadPNG("Images/Dungeon/BottomBombDoorDark.png").FilterRGB(BLACK_R, BLACK_G, BLACK_B);
  dungeonbld = ImageHandler::LoadPNG("Images/Dungeon/DungeonLeftDoorBlack.png").FilterRGB(BLACK_R, BLACK_G, BLACK_B);
  dungeonblk = ImageHandler::LoadPNG("Images/Dungeon/LeftKeyDoorBlack.png").FilterRGB(BLACK_R, BLACK_G, BLACK_B);
  dungeonbls = ImageHandler::LoadPNG("Images/Dungeon/LeftShutterDoorBlack.png").FilterRGB(BLACK_R, BLACK_G, BLACK_B);
  dungeonblb = ImageHandler::LoadPNG("Images/Dungeon/LeftBombDoorBlack.png").FilterRGB(BLACK_R, BLACK_G, BLACK_B);
  dungeonbrd = ImageHandler::LoadPNG("Images/Dungeon/DungeonRightDoorBlack.png").FilterRGB(BLACK_R, BLACK_G, BLACK_B);
  dungeonbrk = ImageHandler::LoadPNG("Images/Dungeon/RightKeyDoorBlack.png").FilterRGB(BLACK_R, BLACK_G, BLACK_B);
  dungeonbrs = ImageHandler::LoadPNG("Images/Dungeon/RightShutterDoorBlack.png").FilterRGB(BLACK_R, BLACK_G, BLACK_B);
  dungeonbrb = ImageHandler::LoadPNG("Images/Dungeon/RightBombDoorBlack.png").FilterRGB(BLACK_R, BLACK_G, BLACK_B);
  dungeonbtd = ImageHandler::LoadPNG("Images/Dungeon/DungeonTopDoorBlack.png").FilterRGB(BLACK_R, BLACK_G, BLACK_B);
  dungeonbtk = ImageHandler::LoadPNG("Images/Dungeon/TopKeyDoorBlack.png").FilterRGB(BLACK_R, BLACK_G, BLACK_B);
  dungeonbts = ImageHandler::LoadPNG("Images/Dungeon/TopShutterDoorBlack.png").FilterRGB(BLACK_R, BLACK_G, BLACK_B);
  dungeonbtb = ImageHandler::LoadPNG("Images/Dungeon/TopBombDoorBlack.png").FilterRGB(BLACK_R, BLACK_G, BLACK_B);
  dungeonbbd = ImageHandler::LoadPNG("Images/Dungeon/DungeonBottomDoorBlack.png").FilterRGB(BLACK_R, BLACK_G, BLACK_B);
  dungeonbbk = ImageHandler::LoadPNG("Images/Dungeon/BottomKeyDoorBlack.png").FilterRGB(BLACK_R, BLACK_G, BLACK_B);
  dungeonbbs = ImageHandler::LoadPNG("Images/Dungeon/BottomShutterDoorBlack.png").FilterRGB(BLACK_R, BLACK_G, BLACK_B);
  dungeonbbb = ImageHandler::LoadPNG("Images/Dungeon/BottomBombDoorBlack.png").FilterRGB(BLACK_R, BLACK_G, BLACK_B);
  dungeondoorhandle = ImageHandler::LoadPNG("Images/Dungeon/DoorHandle.png").FilterRGB(BLACK_R, BLACK_G, BLACK_B);
  dungeondoorhandleblack = ImageHandler::LoadPNG("Images/Dungeon/DoorHandleBlack.png").FilterRGB(BLACK_R, BLACK_G, BLACK_B);
  stairway = ImageHandler::LoadPNG("Images/Dungeon/Stairway.png").FilterRGB(BLACK_R, BLACK_G, BLACK_B);  
  book = ImageHandler::LoadPNG("Images/Dungeon/Book.png").FilterRGB(BLACK_R, BLACK_G, BLACK_B);
  bow = ImageHandler::LoadPNG("Images/Dungeon/Bow.png").FilterRGB(BLACK_R, BLACK_G, BLACK_B);
  heartcontainer = ImageHandler::LoadPNG("Images/Dungeon/HeartContainer.png").FilterRGB(BLACK_R, BLACK_G, BLACK_B);  
  ladder = ImageHandler::LoadPNG("Images/Dungeon/Ladder.png").FilterRGB(BLACK_R, BLACK_G, BLACK_B);
  magicalkey = ImageHandler::LoadPNG("Images/Dungeon/MagicalKey.png").FilterRGB(BLACK_R, BLACK_G, BLACK_B);
  magicalboomerang = ImageHandler::LoadPNG("Images/Dungeon/MagicalBoomerang.png").FilterRGB(BLACK_R, BLACK_G, BLACK_B);
  powerbracelet = ImageHandler::LoadPNG("Images/Dungeon/PowerBracelet.png").FilterRGB(BLACK_R, BLACK_G, BLACK_B);    
  raft = ImageHandler::LoadPNG("Images/Dungeon/Raft.png").FilterRGB(BLACK_R, BLACK_G, BLACK_B);  
  recorder = ImageHandler::LoadPNG("Images/Dungeon/Recorder.png").FilterRGB(BLACK_R, BLACK_G, BLACK_B);
  redcandle = ImageHandler::LoadPNG("Images/Dungeon/RedCandle.png").FilterRGB(BLACK_R, BLACK_G, BLACK_B);
  redring = ImageHandler::LoadPNG("Images/Dungeon/RedRing.png").FilterRGB(BLACK_R, BLACK_G, BLACK_B);
  silverarrow = ImageHandler::LoadPNG("Images/Dungeon/SilverArrow.png").FilterRGB(BLACK_R, BLACK_G, BLACK_B);
  wand = ImageHandler::LoadPNG("Images/Dungeon/Wand.png").FilterRGB(BLACK_R, BLACK_G, BLACK_B);
  whitesworditem = ImageHandler::LoadPNG("Images/Dungeon/WhiteSword.png").FilterRGB(BLACK_R, BLACK_G, BLACK_B);    
  woodenboomerang = ImageHandler::LoadPNG("Images/Dungeon/MagicalBoomerang.png").FilterRGB(BLACK_R, BLACK_G, BLACK_B);  
  heart = ImageHandler::LoadPNG("Images/Dungeon/Heart.png").FilterRGB(BLACK_R, BLACK_G, BLACK_B);    
}

void ZeldaImageProcessor::RecordDungeonNumber(ImageHandler& screen) {
  //check if this is level 9
  ImageHandler lnumber = screen.Crop(REFERENCE_DUNGEON_NUMBER_XCOOR*SCALE_X, REFERENCE_DUNGEON_NUMBER_YCOOR*SCALE_Y, REFERENCE_DUNGEON_NUMBER_WIDTH*SCALE_X, REFERENCE_DUNGEON_NUMBER_HEIGHT*SCALE_Y).ConvertToBlackAndWhite();
  double similarity = 0;
  ZeldaInformationHandler::Secrets dungeon = ZeldaInformationHandler::Secrets::UNKNOWN_DUNGEON;
  if(lnumber.Similarity(dungeonnine) > similarity) {
    similarity = lnumber.Similarity(dungeonnine);
    dungeon = ZeldaInformationHandler::Secrets::DUNGEON_9;
  }
  if(lnumber.Similarity(dungeoneight) > similarity) {
    similarity = lnumber.Similarity(dungeoneight);
    dungeon = ZeldaInformationHandler::Secrets::DUNGEON_8;
  }
  if(lnumber.Similarity(dungeonseven) > similarity) {
    similarity = lnumber.Similarity(dungeonseven);
    dungeon = ZeldaInformationHandler::Secrets::DUNGEON_7;
  }
  if(lnumber.Similarity(dungeonsix) > similarity) {
    similarity = lnumber.Similarity(dungeonsix);
    dungeon = ZeldaInformationHandler::Secrets::DUNGEON_6;
  }
  if(lnumber.Similarity(dungeonfive) > similarity) {
    similarity = lnumber.Similarity(dungeonfive);
    dungeon = ZeldaInformationHandler::Secrets::DUNGEON_5;
  }
  if(lnumber.Similarity(dungeonfour) > similarity) {
    similarity = lnumber.Similarity(dungeonfour);
    dungeon = ZeldaInformationHandler::Secrets::DUNGEON_4;
  }
  if(lnumber.Similarity(dungeonthree) > similarity) {
    similarity = lnumber.Similarity(dungeonthree);
    dungeon = ZeldaInformationHandler::Secrets::DUNGEON_3;
  }
  if(lnumber.Similarity(dungeontwo) > similarity) {
    similarity = lnumber.Similarity(dungeontwo);
    dungeon = ZeldaInformationHandler::Secrets::DUNGEON_2;
  }
  if(lnumber.Similarity(dungeonone) > similarity) {
    similarity = lnumber.Similarity(dungeonone);
    dungeon = ZeldaInformationHandler::Secrets::DUNGEON_1;
  }
  double bp = static_cast<double>(lnumber.PixelsWithRGB(BLACK_R, BLACK_G, BLACK_B).size()) / (lnumber.Width() * lnumber.Height());
  if(bp > similarity) {
    similarity = 0;
    dungeon = ZeldaInformationHandler::Secrets::UNKNOWN_DUNGEON;
  }  
  if (similarity > CAPTURED_DUNGEON_NUMBER_SIMILARITY_THRESHOLD) {
    std::pair<int, int> oloc = ZeldaInformationHandler::GetMapLocation();
    ZeldaInformationHandler::SetSecret(oloc.first, oloc.second, dungeon);
  }
}

std::tuple<bool, int, int> ZeldaImageProcessor::FindLinkDungeon(ImageHandler& minimap) {
  bool foundLink = false;
  int lmapx;
  int lmapy;
  int possibilities = 0;
  for (int mapx = 0; mapx < 8; ++mapx) {
    for (int mapy = 0; mapy < 8; ++mapy) {
      int tx = (static_cast<double>(mapx) / 8) * REFERENCE_OVERWORLD_MINIMAP_WIDTH * SCALE_X;
      int ty = (static_cast<double>(mapy) / 8) * REFERENCE_OVERWORLD_MINIMAP_HEIGHT * SCALE_Y;
      int tw = REFERENCE_OVERWORLD_MINIMAP_WIDTH * SCALE_X / 8;
      int th = REFERENCE_OVERWORLD_MINIMAP_HEIGHT * SCALE_Y / 8;
      int sx = REFERENCE_DUNGEON_MINIMAP_CURSOR_XCOOR * SCALE_X;
      int sy = REFERENCE_DUNGEON_MINIMAP_CURSOR_YCOOR * SCALE_Y;
      int sw = REFERENCE_DUNGEON_MINIMAP_CURSOR_WIDTH * SCALE_X;
      int sh = REFERENCE_DUNGEON_MINIMAP_CURSOR_HEIGHT * SCALE_Y;
      ImageHandler mapbox = minimap.Crop(tx, ty, tw, th);
      ImageHandler mapspot = minimap.Crop(tx, ty, tw, th).Crop(sx, sy, sw, sh);	      
      std::tuple<int, int, int> maprgb = mapspot.MostCommonRGB();
      if (maprgb == std::make_tuple(CURRENT_TUNIC_R, CURRENT_TUNIC_G, CURRENT_TUNIC_B) &&
	  !(ZeldaInformationHandler::GetDungeonRoomType(mapx, mapy) == Dungeon::RoomType::TRIFORCE_ROOM &&
	    std::make_tuple(CURRENT_TUNIC_R, CURRENT_TUNIC_B, CURRENT_TUNIC_G) == std::make_tuple(HEART_RED_R, HEART_RED_B, HEART_RED_G))) {
	mapspot = mapspot.FilterRGB(std::get<0>(maprgb), std::get<1>(maprgb), std::get<2>(maprgb), CAPTURED_DUNGEON_MINIMAP_CURSOR_COLOR_TOLERANCE);
	double b = static_cast<double>(mapspot.PixelsWithRGB(BLACK_R, BLACK_G, BLACK_B).size());
	mapbox = mapbox.FilterRGB(std::get<0>(maprgb), std::get<1>(maprgb), std::get<2>(maprgb), CAPTURED_DUNGEON_MINIMAP_CURSOR_COLOR_TOLERANCE);
	double mb = static_cast<double>(mapbox.PixelsWithRGB(BLACK_R, BLACK_G, BLACK_B).size());
	double bp = b / (mapspot.Width() * mapspot.Height());
	double mbp = (mb-b) / (mapbox.Width() * mapbox.Height());		    
	if (bp > CAPTURED_DUNGEON_MINIMAP_CURSOR_UPPER_BLACK_THRESHOLD && mbp < CAPTURED_DUNGEON_MINIMAP_CURSOR_LOWER_BLACK_THRESHOLD) {
	  possibilities++;
	  lmapx = mapx;
	  lmapy = mapy;
	}		
      }
    }
  }
  if (possibilities == 1) {
    foundLink = true;
  }
  if (possibilities == 2) {
    //case where link has redring and finds shadowless compass
    for (int mapx = 0; mapx < 8; ++mapx) {
      for (int mapy = 0; mapy < 8; ++mapy) {
	int tx = (static_cast<double>(mapx) / 8) * REFERENCE_OVERWORLD_MINIMAP_WIDTH * SCALE_X;
	int ty = (static_cast<double>(mapy) / 8) * REFERENCE_OVERWORLD_MINIMAP_HEIGHT * SCALE_Y;
	int tw = REFERENCE_OVERWORLD_MINIMAP_WIDTH * SCALE_X / 8;
	int th = REFERENCE_OVERWORLD_MINIMAP_HEIGHT * SCALE_Y / 8;
	int sx = REFERENCE_DUNGEON_MINIMAP_CURSOR_XCOOR * SCALE_X;
	int sy = REFERENCE_DUNGEON_MINIMAP_CURSOR_YCOOR * SCALE_Y;
	int sw = REFERENCE_DUNGEON_MINIMAP_CURSOR_WIDTH * SCALE_X;
	int sh = REFERENCE_DUNGEON_MINIMAP_CURSOR_HEIGHT * SCALE_Y;
	ImageHandler mapbox = minimap.Crop(tx, ty, tw, th);
	ImageHandler mapspot = minimap.Crop(tx, ty, tw, th).Crop(sx, sy, sw, sh);	      
	std::tuple<int, int, int> maprgb = mapspot.MostCommonRGB();
	if (maprgb == std::make_tuple(HEART_RED_R, HEART_RED_G, HEART_RED_B)) {
	  mapspot = mapspot.FilterRGB(std::get<0>(maprgb), std::get<1>(maprgb), std::get<2>(maprgb), CAPTURED_DUNGEON_MINIMAP_CURSOR_COLOR_TOLERANCE);
	  double b = static_cast<double>(mapspot.PixelsWithRGB(BLACK_R, BLACK_G, BLACK_B).size());
	  mapbox = mapbox.FilterRGB(std::get<0>(maprgb), std::get<1>(maprgb), std::get<2>(maprgb), CAPTURED_DUNGEON_MINIMAP_CURSOR_COLOR_TOLERANCE);
	  double mb = static_cast<double>(mapbox.PixelsWithRGB(BLACK_R, BLACK_G, BLACK_B).size());
	  double bp = b / (mapspot.Width() * mapspot.Height());
	  double mbp = (mb-b) / (mapbox.Width() * mapbox.Height());		    
	  if (bp > CAPTURED_DUNGEON_MINIMAP_CURSOR_UPPER_BLACK_THRESHOLD && mbp < CAPTURED_DUNGEON_MINIMAP_CURSOR_LOWER_BLACK_THRESHOLD) {
	    if (ZeldaInformationHandler::GetDungeonLocation() == std::make_pair(mapx, mapy)) {
	      lmapx = mapx;
	      lmapy = mapy;
	      foundLink = true;
	    }
	  }		
	}
      }
    }
  }
  return std::make_tuple(foundLink, lmapx, lmapy);
}

void ZeldaImageProcessor::CheckOverworldRing(ImageHandler& minimap) {
  //TODO test for color %
  int possibles = 0;
  std::tuple<int, int, int> color;
  for (int mapx = 0; mapx < 16; ++mapx) {
    for (int mapy = 0; mapy < 8; ++mapy) {
      int tx = (static_cast<double>(mapx) / 16) * REFERENCE_OVERWORLD_MINIMAP_WIDTH * SCALE_X;
      int ty = (static_cast<double>(mapy) / 8) * REFERENCE_OVERWORLD_MINIMAP_HEIGHT * SCALE_Y;
      int tw = REFERENCE_OVERWORLD_MINIMAP_WIDTH * SCALE_X / 16;
      int th = REFERENCE_OVERWORLD_MINIMAP_HEIGHT * SCALE_Y / 8;
      int sx = REFERENCE_OVERWORLD_MINIMAP_CURSOR_XCOOR * SCALE_X;
      int sy = REFERENCE_OVERWORLD_MINIMAP_CURSOR_YCOOR * SCALE_Y;
      int sw = REFERENCE_OVERWORLD_MINIMAP_CURSOR_WIDTH * SCALE_X;
      int sh = REFERENCE_OVERWORLD_MINIMAP_CURSOR_HEIGHT * SCALE_Y;	  
      ImageHandler mapspot = minimap.Crop(tx, ty, tw, th).Crop(sx, sy, sw, sh);	  	    
      std::tuple<int, int, int> maprgb = mapspot.MostCommonRGB();
      std::tuple<int, int, int> minimapGray = std::make_tuple(MINIMAP_GRAY_R, MINIMAP_GRAY_G, MINIMAP_GRAY_B);      
      if (maprgb != std::make_tuple(BLACK_R, BLACK_G, BLACK_B) && maprgb != minimapGray) {
	possibles++;
	color = maprgb;
      }
    }
  }
  if (possibles == 1) {
    CURRENT_TUNIC_R = std::get<0>(color);
    CURRENT_TUNIC_G = std::get<1>(color);
    CURRENT_TUNIC_B = std::get<2>(color);
  }
}

std::tuple<bool, int, int> ZeldaImageProcessor::FindLinkOverworld(ImageHandler& minimap) {
  for (int mapx = 0; mapx < 16; ++mapx) {
    for (int mapy = 0; mapy < 8; ++mapy) {
      int tx = (static_cast<double>(mapx) / 16) * REFERENCE_OVERWORLD_MINIMAP_WIDTH * SCALE_X;
      int ty = (static_cast<double>(mapy) / 8) * REFERENCE_OVERWORLD_MINIMAP_HEIGHT * SCALE_Y;
      int tw = REFERENCE_OVERWORLD_MINIMAP_WIDTH * SCALE_X / 16;
      int th = REFERENCE_OVERWORLD_MINIMAP_HEIGHT * SCALE_Y / 8;
      int sx = REFERENCE_OVERWORLD_MINIMAP_CURSOR_XCOOR * SCALE_X;
      int sy = REFERENCE_OVERWORLD_MINIMAP_CURSOR_YCOOR * SCALE_Y;
      int sw = REFERENCE_OVERWORLD_MINIMAP_CURSOR_WIDTH * SCALE_X;
      int sh = REFERENCE_OVERWORLD_MINIMAP_CURSOR_HEIGHT * SCALE_Y;	  
      ImageHandler mapspot = minimap.Crop(tx, ty, tw, th).Crop(sx, sy, sw, sh);	  
      std::tuple<int, int, int> maprgb = mapspot.MostCommonRGB();
      if (maprgb == std::make_tuple(CURRENT_TUNIC_R, CURRENT_TUNIC_G, CURRENT_TUNIC_B)) {
	return std::make_tuple(true, mapx, mapy);
      }
    }
  }
  return std::make_tuple(false, -1, -1);
}
