#include "ZeldaImageProcessor.h"
#include "ZeldaInformationHandler.h"

ZeldaImageProcessor::ZeldaImageProcessor() {
  while (!FindZeldaScreen()) {
    //Find zelda screen
  }
  ZeldaInformationHandler::SetZeldaSceenFound(true);
}

void ZeldaImageProcessor::PrintDebugData() {
  std::cout << "CAPTURED_REGISTRATION_SCREEN_WIDTH: " << CAPTURED_REGISTRATION_SCREEN_WIDTH << std::endl;
  std::cout << "CAPTURED_REGISTRATION_SCREEN_HEIGHT: " << CAPTURED_REGISTRATION_SCREEN_HEIGHT << std::endl;
  std::cout << "CAPTURED_REGISTRATION_SCREEN_XCOOR: " << CAPTURED_REGISTRATION_SCREEN_XCOOR << std::endl;
  std::cout << "CAPTURED_REGISTRATION_SCREEN_YCOOR: " << CAPTURED_REGISTRATION_SCREEN_YCOOR << std::endl;
  std::cout << "CAPTURED_REGISTRATION_SCREEN_SIMILARITY_THRESHOLD: " <<  CAPTURED_REGISTRATION_SCREEN_SIMILARITY_THRESHOLD << std::endl;

  std::cout << "SCALE_X: " << SCALE_X << std::endl;
  std::cout << "SCALE_Y: " << SCALE_Y << std::endl;

  std::cout << "INITIALIZED_MINIMAP_GRAY: " << INITIALIZED_MINIMAP_GRAY << std::endl;
  std::cout << "INITIALIZED_TUNIC_GREEN: " << INITIALIZED_TUNIC_GREEN << std::endl;
  std::cout << "INITIALIZED_CURRENT_TUNIC: " << INITIALIZED_CURRENT_TUNIC << std::endl;
  
  std::cout << "TUNIC_GREEN_R: " << TUNIC_GREEN_R << std::endl;
  std::cout << "TUNIC_GREEN_G: " << TUNIC_GREEN_G << std::endl;
  std::cout << "TUNIC_GREEN_B: " << TUNIC_GREEN_B << std::endl;

  std::cout << "MINIMAP_GRAY_R: " << MINIMAP_GRAY_R << std::endl;
  std::cout << "MINIMAP_GRAY_G: " << MINIMAP_GRAY_G << std::endl;
  std::cout << "MINIMAP_GRAY_B: " << MINIMAP_GRAY_B << std::endl;

  std::cout << "HEART_RED_R: " << HEART_RED_R << std::endl;
  std::cout << "HEART_RED_G: " << HEART_RED_G << std::endl;
  std::cout << "HEART_RED_B: " << HEART_RED_B << std::endl;

  std::cout << "START_BLUE_R: " << START_BLUE_R << std::endl;
  std::cout << "START_BLUE_G: " << START_BLUE_G << std::endl;
  std::cout << "START_BLUE_B: " << START_BLUE_B << std::endl;

  std::cout << "CURRENT_TUNIC_R: " << CURRENT_TUNIC_R << std::endl;
  std::cout << "CURRENT_TUNIC_G: " << CURRENT_TUNIC_G << std::endl;
  std::cout << "CURRENT_TUNIC_B: " << CURRENT_TUNIC_B << std::endl;

  std::cout << std::endl;
}

void ZeldaImageProcessor::UpdateData() {
  ImageHandler screen = GetScreen();
  if (!INITIALIZED_MINIMAP_GRAY) {
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
  if (INITIALIZED_MINIMAP_GRAY && !INITIALIZED_TUNIC_GREEN) {
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
  if (INITIALIZED_CURRENT_TUNIC) {
    //Search for Link's location
    ImageHandler minimap = screen.Crop(REFERENCE_OVERWORLD_MINIMAP_XCOOR*SCALE_X, REFERENCE_OVERWORLD_MINIMAP_YCOOR*SCALE_Y, REFERENCE_OVERWORLD_MINIMAP_WIDTH*SCALE_X, REFERENCE_OVERWORLD_MINIMAP_HEIGHT*SCALE_Y);
    std::tuple<int, int, int> rgb = minimap.MostCommonRGB();
    std::tuple<int, int, int> minimapGray = std::make_tuple(MINIMAP_GRAY_R, MINIMAP_GRAY_G, MINIMAP_GRAY_B);
    if (rgb == minimapGray) {
      //determine location
      bool foundLink = false;
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
	    //location found
	    foundLink = true;
	    ZeldaInformationHandler::SetMapLocation(mapx, mapy);
	    //check if in secret cave
	    ImageHandler playScreen = screen.Crop(REFERENCE_PLAYING_SCREEN_XCOOR*SCALE_X, REFERENCE_PLAYING_SCREEN_YCOOR*SCALE_Y, REFERENCE_PLAYING_SCREEN_WIDTH*SCALE_X, REFERENCE_PLAYING_SCREEN_HEIGHT*SCALE_Y);
	    double bp = static_cast<double>(playScreen.PixelsWithRGB(BLACK_R, BLACK_G, BLACK_B).size()) / (playScreen.Width() * playScreen.Height());
	    bool inSecretCave = (bp > SECRET_CAVE_BLACK_THRESHOLD);
	    if (inSecretCave) {
	      ZeldaInformationHandler::SetSecret(mapx, mapy, ZeldaInformationHandler::Secrets::UNKNOWN_CAVE);
	    }
	  }
	}
      }
      if (!foundLink) {
      	//check for blue or red ring
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
      	    if (maprgb != std::make_tuple(BLACK_R, BLACK_G, BLACK_B) && maprgb != minimapGray && screen.PixelsWithRGB(CURRENT_TUNIC_R, CURRENT_TUNIC_G, CURRENT_TUNIC_B).size() == 0) {
      	      CURRENT_TUNIC_R = std::get<0>(maprgb);
      	      CURRENT_TUNIC_G = std::get<1>(maprgb);
      	      CURRENT_TUNIC_B = std::get<2>(maprgb);
      	    }
      	  }
      	}
      }
    }
    else {
      //check for dungeon location
      int grayCount = minimap.PixelsWithRGB(MINIMAP_GRAY_R, MINIMAP_GRAY_G, MINIMAP_GRAY_B).size();
      if (grayCount == 0) {
	bool foundLink = false;
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
	    if (maprgb == std::make_tuple(CURRENT_TUNIC_R, CURRENT_TUNIC_G, CURRENT_TUNIC_B)) {
	      foundLink = true;
	      ZeldaInformationHandler::RoomType type = ZeldaInformationHandler::GetDungeonRoomType(mapx, mapy);
	      if (type == ZeldaInformationHandler::RoomType::UNEXPLORED || type == ZeldaInformationHandler::RoomType::UNSEEN_ROOM) {
		//check for doors
		{
		  ImageHandler dl = screen.Crop(REFERENCE_LEFT_DUNGEON_DOOR_XCOOR*SCALE_X, REFERENCE_LEFT_DUNGEON_DOOR_YCOOR*SCALE_Y, REFERENCE_LEFT_DUNGEON_DOOR_WIDTH*SCALE_X, REFERENCE_LEFT_DUNGEON_DOOR_HEIGHT*SCALE_Y);
		  {
		    ImageHandler dld = dl.FilterRGB(BLACK_R, BLACK_G, BLACK_B);
		    ImageHandler zeldaDLD = ImageHandler::LoadPNG("Images/Dungeon/DungeonLeftDoor.png").FilterRGB(BLACK_R, BLACK_G, BLACK_B);
		    double simDLD = std::min(zeldaDLD.Similarity(dld), dld.Similarity(zeldaDLD));
		    if (simDLD > CAPTURED_LEFT_DOOR_SIMILARITY_THRESHOLD) {
		      ZeldaInformationHandler::SetDungeonLocation(mapx-1, mapy, ZeldaInformationHandler::RoomType::UNSEEN_ROOM);
		      ZeldaInformationHandler::SetDungeonDoor(mapx, mapy, mapx-1, mapy, ZeldaInformationHandler::DoorType::OPEN);
		    }
		  }
		  double keySim;
		  {
		    std::tuple<int, int, int> color = dl.MostCommonRGB();
		    ImageHandler dlk = dl.FilterRGB(std::get<0>(color), std::get<1>(color), std::get<2>(color));
		    ImageHandler zeldaDLK = ImageHandler::LoadPNG("Images/Dungeon/LeftKeyDoor.png");
		    std::tuple<int, int, int> zcolor = zeldaDLK.MostCommonRGB();
		    ImageHandler zdlk = zeldaDLK.FilterRGB(std::get<0>(zcolor), std::get<1>(zcolor), std::get<2>(zcolor));
		    double simDLK = std::min(zdlk.Similarity(dlk), dlk.Similarity(zdlk));
		    if (simDLK > CAPTURED_LEFT_KEY_DOOR_SIMILARITY_THRESHOLD) {
		      ZeldaInformationHandler::SetDungeonLocation(mapx-1, mapy, ZeldaInformationHandler::RoomType::UNSEEN_ROOM);
		      ZeldaInformationHandler::SetDungeonDoor(mapx, mapy, mapx-1, mapy, ZeldaInformationHandler::DoorType::KEY);
		    }
		    keySim = simDLK;
		  }
		  {
		    std::tuple<int, int, int> color = dl.MostCommonRGB();
		    ImageHandler dlk = dl.FilterRGB(std::get<0>(color), std::get<1>(color), std::get<2>(color));
		    ImageHandler zeldaDLK = ImageHandler::LoadPNG("Images/Dungeon/LeftShutterDoor.png");
		    std::tuple<int, int, int> zcolor = zeldaDLK.MostCommonRGB();
		    ImageHandler zdlk = zeldaDLK.FilterRGB(std::get<0>(zcolor), std::get<1>(zcolor), std::get<2>(zcolor));
		    double simDLK = std::min(zdlk.Similarity(dlk), dlk.Similarity(zdlk));
		    if (simDLK > CAPTURED_LEFT_SHUTTER_DOOR_SIMILARITY_THRESHOLD && simDLK > keySim) {
		      ZeldaInformationHandler::SetDungeonLocation(mapx-1, mapy, ZeldaInformationHandler::RoomType::UNSEEN_ROOM);
		      ZeldaInformationHandler::SetDungeonDoor(mapx, mapy, mapx-1, mapy, ZeldaInformationHandler::DoorType::SHUTTER);
		    }
		  }
		  {
		    ImageHandler dl = screen.Crop(REFERENCE_LEFT_DUNGEON_DOOR_XCOOR*SCALE_X, REFERENCE_LEFT_DUNGEON_DOOR_YCOOR*SCALE_Y, REFERENCE_LEFT_DUNGEON_DOOR_WIDTH*SCALE_X, REFERENCE_LEFT_DUNGEON_DOOR_HEIGHT*SCALE_Y);
		    ImageHandler dld = dl.FilterRGB(BLACK_R, BLACK_G, BLACK_B);
		    ImageHandler zeldaDLD = ImageHandler::LoadPNG("Images/Dungeon/LeftBombDoor.png").FilterRGB(BLACK_R, BLACK_G, BLACK_B);
		    double simDLD = std::min(zeldaDLD.Similarity(dld), dld.Similarity(zeldaDLD));
		    if (simDLD > CAPTURED_LEFT_BOMB_DOOR_SIMILARITY_THRESHOLD) {
		      ZeldaInformationHandler::SetDungeonLocation(mapx-1, mapy, ZeldaInformationHandler::RoomType::UNSEEN_ROOM);
		      ZeldaInformationHandler::SetDungeonDoor(mapx, mapy, mapx-1, mapy, ZeldaInformationHandler::DoorType::BOMB);
		    }
		  }
		}
		{
		  ImageHandler dl = screen.Crop(REFERENCE_RIGHT_DUNGEON_DOOR_XCOOR*SCALE_X, REFERENCE_RIGHT_DUNGEON_DOOR_YCOOR*SCALE_Y, REFERENCE_RIGHT_DUNGEON_DOOR_WIDTH*SCALE_X, REFERENCE_RIGHT_DUNGEON_DOOR_HEIGHT*SCALE_Y);
		  {
		    ImageHandler dld = dl.FilterRGB(BLACK_R, BLACK_G, BLACK_B);
		    ImageHandler zeldaDLD = ImageHandler::LoadPNG("Images/Dungeon/DungeonRightDoor.png").FilterRGB(BLACK_R, BLACK_G, BLACK_B);
		    double simDLD = std::min(zeldaDLD.Similarity(dld), dld.Similarity(zeldaDLD));
		    if (simDLD > CAPTURED_RIGHT_DOOR_SIMILARITY_THRESHOLD) {
		      ZeldaInformationHandler::SetDungeonLocation(mapx+1, mapy, ZeldaInformationHandler::RoomType::UNSEEN_ROOM);
		      ZeldaInformationHandler::SetDungeonDoor(mapx, mapy, mapx+1, mapy, ZeldaInformationHandler::DoorType::OPEN);
		    }
		  }
		  double keySim;
		  {
		    std::tuple<int, int, int> color = dl.MostCommonRGB();
		    ImageHandler dlk = dl.FilterRGB(std::get<0>(color), std::get<1>(color), std::get<2>(color));
		    ImageHandler zeldaDLK = ImageHandler::LoadPNG("Images/Dungeon/RightKeyDoor.png");
		    std::tuple<int, int, int> zcolor = zeldaDLK.MostCommonRGB();
		    ImageHandler zdlk = zeldaDLK.FilterRGB(std::get<0>(zcolor), std::get<1>(zcolor), std::get<2>(zcolor));
		    double simDLK = std::min(zdlk.Similarity(dlk), dlk.Similarity(zdlk));
		    if (simDLK > CAPTURED_RIGHT_KEY_DOOR_SIMILARITY_THRESHOLD) {
		      ZeldaInformationHandler::SetDungeonLocation(mapx+1, mapy, ZeldaInformationHandler::RoomType::UNSEEN_ROOM);
		      ZeldaInformationHandler::SetDungeonDoor(mapx, mapy, mapx+1, mapy, ZeldaInformationHandler::DoorType::KEY);
		    }
		    keySim = simDLK;
		  }		  
		  {
		    std::tuple<int, int, int> color = dl.MostCommonRGB();
		    ImageHandler dlk = dl.FilterRGB(std::get<0>(color), std::get<1>(color), std::get<2>(color));
		    ImageHandler zeldaDLK = ImageHandler::LoadPNG("Images/Dungeon/RightShutterDoor.png");
		    std::tuple<int, int, int> zcolor = zeldaDLK.MostCommonRGB();
		    ImageHandler zdlk = zeldaDLK.FilterRGB(std::get<0>(zcolor), std::get<1>(zcolor), std::get<2>(zcolor));
		    double simDLK = std::min(zdlk.Similarity(dlk), dlk.Similarity(zdlk));
		    if (simDLK > CAPTURED_RIGHT_SHUTTER_DOOR_SIMILARITY_THRESHOLD && simDLK > keySim) {
		      ZeldaInformationHandler::SetDungeonLocation(mapx+1, mapy, ZeldaInformationHandler::RoomType::UNSEEN_ROOM);
		      ZeldaInformationHandler::SetDungeonDoor(mapx, mapy, mapx+1, mapy, ZeldaInformationHandler::DoorType::SHUTTER);
		    }
		  }
		  {
		    ImageHandler dl = screen.Crop(REFERENCE_RIGHT_DUNGEON_DOOR_XCOOR*SCALE_X, REFERENCE_RIGHT_DUNGEON_DOOR_YCOOR*SCALE_Y, REFERENCE_RIGHT_DUNGEON_DOOR_WIDTH*SCALE_X, REFERENCE_RIGHT_DUNGEON_DOOR_HEIGHT*SCALE_Y);
		    ImageHandler dld = dl.FilterRGB(BLACK_R, BLACK_G, BLACK_B);
		    ImageHandler zeldaDLD = ImageHandler::LoadPNG("Images/Dungeon/RightBombDoor.png").FilterRGB(BLACK_R, BLACK_G, BLACK_B);
		    double simDLD = std::min(zeldaDLD.Similarity(dld), dld.Similarity(zeldaDLD));
		    if (simDLD > CAPTURED_RIGHT_BOMB_DOOR_SIMILARITY_THRESHOLD) {
		      ZeldaInformationHandler::SetDungeonLocation(mapx+1, mapy, ZeldaInformationHandler::RoomType::UNSEEN_ROOM);
		      ZeldaInformationHandler::SetDungeonDoor(mapx, mapy, mapx+1, mapy, ZeldaInformationHandler::DoorType::BOMB);
		    }
		  }
		}
		{
		  ImageHandler dl = screen.Crop(REFERENCE_TOP_DUNGEON_DOOR_XCOOR*SCALE_X, REFERENCE_TOP_DUNGEON_DOOR_YCOOR*SCALE_Y, REFERENCE_TOP_DUNGEON_DOOR_WIDTH*SCALE_X, REFERENCE_TOP_DUNGEON_DOOR_HEIGHT*SCALE_Y);
		  {
		    ImageHandler dld = dl.FilterRGB(BLACK_R, BLACK_G, BLACK_B);
		    ImageHandler zeldaDLD = ImageHandler::LoadPNG("Images/Dungeon/DungeonTopDoor.png").FilterRGB(BLACK_R, BLACK_G, BLACK_B);
		    double simDLD = std::min(zeldaDLD.Similarity(dld), dld.Similarity(zeldaDLD));
		    if (simDLD > CAPTURED_TOP_DOOR_SIMILARITY_THRESHOLD) {
		      ZeldaInformationHandler::SetDungeonLocation(mapx, mapy-1, ZeldaInformationHandler::RoomType::UNSEEN_ROOM);
		      ZeldaInformationHandler::SetDungeonDoor(mapx, mapy, mapx, mapy-1, ZeldaInformationHandler::DoorType::OPEN);
		    }
		  }
		  double keySim;
		  {
		    std::tuple<int, int, int> color = dl.MostCommonRGB();
		    ImageHandler dlk = dl.FilterRGB(std::get<0>(color), std::get<1>(color), std::get<2>(color));
		    ImageHandler zeldaDLK = ImageHandler::LoadPNG("Images/Dungeon/TopKeyDoor.png");
		    std::tuple<int, int, int> zcolor = zeldaDLK.MostCommonRGB();
		    ImageHandler zdlk = zeldaDLK.FilterRGB(std::get<0>(zcolor), std::get<1>(zcolor), std::get<2>(zcolor));
		    double simDLK = std::min(zdlk.Similarity(dlk), dlk.Similarity(zdlk));
		    if (simDLK > CAPTURED_TOP_KEY_DOOR_SIMILARITY_THRESHOLD) {
		      ZeldaInformationHandler::SetDungeonLocation(mapx, mapy-1, ZeldaInformationHandler::RoomType::UNSEEN_ROOM);
		      ZeldaInformationHandler::SetDungeonDoor(mapx, mapy, mapx, mapy-1, ZeldaInformationHandler::DoorType::KEY);
		    }
		    keySim = simDLK;
		  }
		  {
		    std::tuple<int, int, int> color = dl.MostCommonRGB();
		    ImageHandler dlk = dl.FilterRGB(std::get<0>(color), std::get<1>(color), std::get<2>(color));
		    ImageHandler zeldaDLK = ImageHandler::LoadPNG("Images/Dungeon/TopShutterDoor.png");
		    std::tuple<int, int, int> zcolor = zeldaDLK.MostCommonRGB();
		    ImageHandler zdlk = zeldaDLK.FilterRGB(std::get<0>(zcolor), std::get<1>(zcolor), std::get<2>(zcolor));
		    double simDLK = std::min(zdlk.Similarity(dlk), dlk.Similarity(zdlk));
		    if (simDLK > CAPTURED_TOP_SHUTTER_DOOR_SIMILARITY_THRESHOLD && simDLK > keySim) {
		      ZeldaInformationHandler::SetDungeonLocation(mapx, mapy-1, ZeldaInformationHandler::RoomType::UNSEEN_ROOM);
		      ZeldaInformationHandler::SetDungeonDoor(mapx, mapy, mapx, mapy-1, ZeldaInformationHandler::DoorType::SHUTTER);
		    }
		  }
		  {
		    ImageHandler dl = screen.Crop(REFERENCE_TOP_DUNGEON_DOOR_XCOOR*SCALE_X, REFERENCE_TOP_DUNGEON_DOOR_YCOOR*SCALE_Y, REFERENCE_TOP_DUNGEON_DOOR_WIDTH*SCALE_X, REFERENCE_TOP_DUNGEON_DOOR_HEIGHT*SCALE_Y);
		    ImageHandler dld = dl.FilterRGB(BLACK_R, BLACK_G, BLACK_B);
		    ImageHandler zeldaDLD = ImageHandler::LoadPNG("Images/Dungeon/TopBombDoor.png").FilterRGB(BLACK_R, BLACK_G, BLACK_B);
		    double simDLD = std::min(zeldaDLD.Similarity(dld), dld.Similarity(zeldaDLD));
		    if (simDLD > CAPTURED_TOP_BOMB_DOOR_SIMILARITY_THRESHOLD) {
		      ZeldaInformationHandler::SetDungeonLocation(mapx, mapy-1, ZeldaInformationHandler::RoomType::UNSEEN_ROOM);
		      ZeldaInformationHandler::SetDungeonDoor(mapx, mapy, mapx, mapy-1, ZeldaInformationHandler::DoorType::BOMB);
		    }
		  }
		}
		{
		  ImageHandler dl = screen.Crop(REFERENCE_BOTTOM_DUNGEON_DOOR_XCOOR*SCALE_X, REFERENCE_BOTTOM_DUNGEON_DOOR_YCOOR*SCALE_Y, REFERENCE_BOTTOM_DUNGEON_DOOR_WIDTH*SCALE_X, REFERENCE_BOTTOM_DUNGEON_DOOR_HEIGHT*SCALE_Y);
		  {
		    ImageHandler dld = dl.FilterRGB(BLACK_R, BLACK_G, BLACK_B);
		    ImageHandler zeldaDLD = ImageHandler::LoadPNG("Images/Dungeon/DungeonBottomDoor.png").FilterRGB(BLACK_R, BLACK_G, BLACK_B);
		    double simDLD = std::min(zeldaDLD.Similarity(dld), dld.Similarity(zeldaDLD));
		    if (simDLD > CAPTURED_BOTTOM_DOOR_SIMILARITY_THRESHOLD) {
		      ZeldaInformationHandler::SetDungeonLocation(mapx, mapy+1, ZeldaInformationHandler::RoomType::UNSEEN_ROOM);
		      ZeldaInformationHandler::SetDungeonDoor(mapx, mapy, mapx, mapy+1, ZeldaInformationHandler::DoorType::OPEN);
		    }
		  }
		  double keySim;
		  {
		    std::tuple<int, int, int> color = dl.MostCommonRGB();
		    ImageHandler dlk = dl.FilterRGB(std::get<0>(color), std::get<1>(color), std::get<2>(color));
		    ImageHandler zeldaDLK = ImageHandler::LoadPNG("Images/Dungeon/BottomKeyDoor.png");
		    std::tuple<int, int, int> zcolor = zeldaDLK.MostCommonRGB();
		    ImageHandler zdlk = zeldaDLK.FilterRGB(std::get<0>(zcolor), std::get<1>(zcolor), std::get<2>(zcolor));
		    double simDLK = std::min(zdlk.Similarity(dlk), dlk.Similarity(zdlk));
		    if (simDLK > CAPTURED_BOTTOM_KEY_DOOR_SIMILARITY_THRESHOLD) {
		      ZeldaInformationHandler::SetDungeonLocation(mapx, mapy+1, ZeldaInformationHandler::RoomType::UNSEEN_ROOM);
		      ZeldaInformationHandler::SetDungeonDoor(mapx, mapy, mapx, mapy+1, ZeldaInformationHandler::DoorType::KEY);
		    }
		    keySim = simDLK;
		  }
		  {
		    std::tuple<int, int, int> color = dl.MostCommonRGB();
		    ImageHandler dlk = dl.FilterRGB(std::get<0>(color), std::get<1>(color), std::get<2>(color));
		    ImageHandler zeldaDLK = ImageHandler::LoadPNG("Images/Dungeon/BottomShutterDoor.png");
		    std::tuple<int, int, int> zcolor = zeldaDLK.MostCommonRGB();
		    ImageHandler zdlk = zeldaDLK.FilterRGB(std::get<0>(zcolor), std::get<1>(zcolor), std::get<2>(zcolor));
		    double simDLK = std::min(zdlk.Similarity(dlk), dlk.Similarity(zdlk));
		    if (simDLK > CAPTURED_BOTTOM_SHUTTER_DOOR_SIMILARITY_THRESHOLD && simDLK > keySim) {
		      ZeldaInformationHandler::SetDungeonLocation(mapx, mapy+1, ZeldaInformationHandler::RoomType::UNSEEN_ROOM);
		      ZeldaInformationHandler::SetDungeonDoor(mapx, mapy, mapx, mapy+1, ZeldaInformationHandler::DoorType::SHUTTER);
		    }
		  }
		}
		{
		  ImageHandler dl = screen.Crop(REFERENCE_BOTTOM_DUNGEON_DOOR_XCOOR*SCALE_X, REFERENCE_BOTTOM_DUNGEON_DOOR_YCOOR*SCALE_Y, REFERENCE_BOTTOM_DUNGEON_DOOR_WIDTH*SCALE_X, REFERENCE_BOTTOM_DUNGEON_DOOR_HEIGHT*SCALE_Y);
		  ImageHandler dld = dl.FilterRGB(BLACK_R, BLACK_G, BLACK_B);
		  ImageHandler zeldaDLD = ImageHandler::LoadPNG("Images/Dungeon/BottomBombDoor.png").FilterRGB(BLACK_R, BLACK_G, BLACK_B);
		  double simDLD = std::min(zeldaDLD.Similarity(dld), dld.Similarity(zeldaDLD));
		  if (simDLD > CAPTURED_BOTTOM_BOMB_DOOR_SIMILARITY_THRESHOLD) {
		    ZeldaInformationHandler::SetDungeonLocation(mapx, mapy+1, ZeldaInformationHandler::RoomType::UNSEEN_ROOM);
		    ZeldaInformationHandler::SetDungeonDoor(mapx, mapy, mapx, mapy+1, ZeldaInformationHandler::DoorType::BOMB);
		  }
		}
	      }
	      ZeldaInformationHandler::SetDungeonLocation(mapx, mapy, ZeldaInformationHandler::RoomType::UNKNOWN_ROOM);
	    }
	  }
	}
	if (!foundLink && (minimap.PixelsWithRGB(HEART_RED_R, HEART_RED_G, HEART_RED_B).size() == 0) && (minimap.PixelsWithRGB(WHITE_R, WHITE_G, WHITE_B).size() == 0) && (minimap.PixelsWithRGB(START_BLUE_R, START_BLUE_G, START_BLUE_B).size() == 0) && screen.PixelsWithRGB(CURRENT_TUNIC_R, CURRENT_TUNIC_G, CURRENT_TUNIC_B).size() == 0) {
	  //check for heart red, start blue,  and white pixels to discount possibility of start screen
	  int possibles = 0;
	  std::tuple<int, int, int> color;
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
	      if (maprgb != std::make_tuple(BLACK_R, BLACK_G, BLACK_B)) {
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
      }
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
      auto components = ConnectedComponents(coordinates);
      ImageHandler zeldaC = ImageHandler::LoadPNG("Images/Registration/RegistrationScreenC.png");
      ImageHandler zeldaG = ImageHandler::LoadPNG("Images/Registration/RegistrationScreenG.png");
      for (size_t i = 0; i < components.size(); ++i) {
	std::vector<int> box = BoundingBox(components[i]);      
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
    std::vector<std::pair<int,int>> blackcoordinates = screen.PixelsWithRGB(BLACK_R, BLACK_G, BLACK_B);
    std::vector<int> blackbox = BoundingBox(blackcoordinates);
    screen = screen.Crop(blackbox[0], blackbox[1], blackbox[2], blackbox[3]);
    std::vector<std::pair<int,int>> coordinates = screen.PixelsWithRGB(WHITE_R, WHITE_G, WHITE_B);
    if (coordinates.empty()) {
      return false;
    }
    else {
      auto components = ConnectedComponents(coordinates);
      ImageHandler zeldaC = ImageHandler::LoadPNG("Images/Registration/RegistrationScreenC.png");
      ImageHandler zeldaG = ImageHandler::LoadPNG("Images/Registration/RegistrationScreenG.png");
      double maxSimC = 0;
      size_t maxSimIndexC = 0;
      double maxSimG = 0;
      size_t maxSimIndexG = 0;
      for (size_t i = 0; i < components.size(); ++i) {
	std::vector<int> box = BoundingBox(components[i]);      
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
	std::vector<int> boxF = BoundingBox(components[maxSimIndexC]);
	double fx = boxF[0];
	double fy = boxF[1];
	std::vector<int> boxG = BoundingBox(components[maxSimIndexG]);
	double gx = boxG[0];
	double gy = boxG[1];
	SCALE_X = (gx-fx) / static_cast<double>(REFERENCE_REGISTRATION_SCREEN_G_XCOOR - REFERENCE_REGISTRATION_SCREEN_C_XCOOR);
	SCALE_Y = (gy-fy) / static_cast<double>(REFERENCE_REGISTRATION_SCREEN_G_YCOOR - REFERENCE_REGISTRATION_SCREEN_C_YCOOR);
	CAPTURED_REGISTRATION_SCREEN_WIDTH = SCALE_X * REFERENCE_SCREEN_WIDTH;
	CAPTURED_REGISTRATION_SCREEN_HEIGHT = SCALE_Y * REFERENCE_SCREEN_HEIGHT;
	CAPTURED_REGISTRATION_SCREEN_XCOOR = gx - SCALE_X * REFERENCE_REGISTRATION_SCREEN_G_XCOOR + blackbox[0];
	CAPTURED_REGISTRATION_SCREEN_YCOOR = gy - SCALE_Y * REFERENCE_REGISTRATION_SCREEN_G_YCOOR + blackbox[1];
      }
      else{
	return false;
      }
    }
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

std::vector<std::vector<std::pair<int,int>>> ZeldaImageProcessor::ConnectedComponents(std::vector<std::pair<int,int>> coordinates) {
  std::vector<std::vector<std::pair<int,int>>> components;
  std::vector<std::pair<int,int>> openSet;
  while (!coordinates.empty()) {
    std::vector<std::pair<int,int>> newComponent;
    std::pair<int,int> start = coordinates[0];
    openSet.push_back(start);
    coordinates.erase(coordinates.begin());
    while (!openSet.empty()) {
      std::pair<int,int> pixel = openSet[0];
      openSet.erase(openSet.begin());
      newComponent.push_back(pixel);
      std::pair<int,int> upixel = std::make_pair(pixel.first, pixel.second-1);
      std::pair<int,int> dpixel = std::make_pair(pixel.first, pixel.second+1);
      std::pair<int,int> lpixel = std::make_pair(pixel.first-1, pixel.second);
      std::pair<int,int> rpixel = std::make_pair(pixel.first+1, pixel.second);
      if (std::find(coordinates.begin(), coordinates.end(), upixel) != coordinates.end()
	  && std::find(openSet.begin(), openSet.end(), upixel) == openSet.end()
	  && std::find(newComponent.begin(), newComponent.end(), upixel) == newComponent.end()) {
	openSet.push_back(upixel);
	coordinates.erase(std::find(coordinates.begin(), coordinates.end(), upixel));
      }
      if (std::find(coordinates.begin(), coordinates.end(), dpixel) != coordinates.end()
	  && std::find(openSet.begin(), openSet.end(), dpixel) == openSet.end()
	  && std::find(newComponent.begin(), newComponent.end(), dpixel) == newComponent.end()) {
	openSet.push_back(dpixel);
	coordinates.erase(std::find(coordinates.begin(), coordinates.end(), dpixel));	
      }
      if (std::find(coordinates.begin(), coordinates.end(), lpixel) != coordinates.end()
	  && std::find(openSet.begin(), openSet.end(), lpixel) == openSet.end()
	  && std::find(newComponent.begin(), newComponent.end(), lpixel) == newComponent.end()) {
	openSet.push_back(lpixel);
	coordinates.erase(std::find(coordinates.begin(), coordinates.end(), lpixel));	
      }
      if (std::find(coordinates.begin(), coordinates.end(), rpixel) != coordinates.end()
	  && std::find(openSet.begin(), openSet.end(), rpixel) == openSet.end()
	  && std::find(newComponent.begin(), newComponent.end(), rpixel) == newComponent.end()) {
	openSet.push_back(rpixel);
	coordinates.erase(std::find(coordinates.begin(), coordinates.end(), rpixel));	
      }
    }
    components.push_back(newComponent);
  }
  return components;
}

