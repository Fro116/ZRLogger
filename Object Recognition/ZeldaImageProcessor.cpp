#include "ZeldaImageProcessor.h"
#include "ZeldaInformationHandler.h"

ZeldaImageProcessor::ZeldaImageProcessor() {  
  while (!FindZeldaScreen()) {
    //Find zelda screen
  }
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
  dungeonbd = ImageHandler::LoadPNG("Images/Dungeon/DungeonRightDoor.png").FilterRGB(BLACK_R, BLACK_G, BLACK_B);
  dungeonbk = ImageHandler::LoadPNG("Images/Dungeon/RightKeyDoor.png");
  dungeonbs = ImageHandler::LoadPNG("Images/Dungeon/RightShutterDoor.png");
  dungeonbb = ImageHandler::LoadPNG("Images/Dungeon/RightBombDoor.png").FilterRGB(BLACK_R, BLACK_G, BLACK_B);
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
  dungeondbd = ImageHandler::LoadPNG("Images/Dungeon/DungeonRightDoorDark.png").FilterRGB(BLACK_R, BLACK_G, BLACK_B);
  dungeondbk = ImageHandler::LoadPNG("Images/Dungeon/RightKeyDoorDark.png").FilterRGB(BLACK_R, BLACK_G, BLACK_B);
  dungeondbs = ImageHandler::LoadPNG("Images/Dungeon/RightShutterDoorDark.png").FilterRGB(BLACK_R, BLACK_G, BLACK_B);
  dungeondbb = ImageHandler::LoadPNG("Images/Dungeon/RightBombDoorDark.png").FilterRGB(BLACK_R, BLACK_G, BLACK_B);
  dungeondoorhandle = ImageHandler::LoadPNG("Images/Dungeon/DoorHandle.png").FilterRGB(BLACK_R, BLACK_G, BLACK_B);
  book = ImageHandler::LoadPNG("Images/Dungeon/Book.png").FilterRGB(BLACK_R, BLACK_G, BLACK_B);
  bow = ImageHandler::LoadPNG("Images/Dungeon/Bow.png").FilterRGB(BLACK_R, BLACK_G, BLACK_B);
  ladder = ImageHandler::LoadPNG("Images/Dungeon/Ladder.png").FilterRGB(BLACK_R, BLACK_G, BLACK_B);
  magicalkey = ImageHandler::LoadPNG("Images/Dungeon/MagicalKey.png").FilterRGB(BLACK_R, BLACK_G, BLACK_B);
  raft = ImageHandler::LoadPNG("Images/Dungeon/Raft.png").FilterRGB(BLACK_R, BLACK_G, BLACK_B);  
  recorder = ImageHandler::LoadPNG("Images/Dungeon/Recorder.png").FilterRGB(BLACK_R, BLACK_G, BLACK_B);
  redcandle = ImageHandler::LoadPNG("Images/Dungeon/RedCandle.png").FilterRGB(BLACK_R, BLACK_G, BLACK_B);
  redring = ImageHandler::LoadPNG("Images/Dungeon/RedRing.png").FilterRGB(BLACK_R, BLACK_G, BLACK_B);
  silverarrow = ImageHandler::LoadPNG("Images/Dungeon/SilverArrow.png").FilterRGB(BLACK_R, BLACK_G, BLACK_B);
  wand = ImageHandler::LoadPNG("Images/Dungeon/Wand.png").FilterRGB(BLACK_R, BLACK_G, BLACK_B);
  
  ZeldaInformationHandler::SetZeldaSceenFound(true);
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
    //first check to see if you are on the start screen or not by aligning to the first dash in "-LIFE-"
    std::tuple<int, int, int> heartrgb = screen.Crop(REFERENCE_HEART_RED_XCOOR*SCALE_X, REFERENCE_HEART_RED_YCOOR*SCALE_Y, REFERENCE_HEART_RED_WIDTH*SCALE_X, REFERENCE_HEART_RED_HEIGHT*SCALE_Y).MostCommonRGB();    
    if (heartrgb == std::make_tuple(HEART_RED_R, HEART_RED_G, HEART_RED_B)) {
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
	      dungeondoortransition = false;
	      //check if in secret cave
	      ImageHandler playScreen = screen.Crop(REFERENCE_PLAYING_SCREEN_XCOOR*SCALE_X, REFERENCE_PLAYING_SCREEN_YCOOR*SCALE_Y, REFERENCE_PLAYING_SCREEN_WIDTH*SCALE_X, REFERENCE_PLAYING_SCREEN_HEIGHT*SCALE_Y);
	      double bp = static_cast<double>(playScreen.PixelsWithRGB(BLACK_R, BLACK_G, BLACK_B).size()) / (playScreen.Width() * playScreen.Height());	      
	      ImageHandler shopOwner = screen.Crop(REFERENCE_SHOP_OWNER_XCOOR*SCALE_X, REFERENCE_SHOP_OWNER_YCOOR*SCALE_Y, REFERENCE_SHOP_OWNER_WIDTH*SCALE_X, REFERENCE_SHOP_OWNER_HEIGHT*SCALE_Y);
	      double sobp = static_cast<double>(shopOwner.PixelsWithRGB(BLACK_R, BLACK_G, BLACK_B).size()) / (shopOwner.Width() * shopOwner.Height());
	      bool inSecretCave = (bp > SECRET_CAVE_BLACK_THRESHOLD) && (sobp < SHOP_OWNER_BLACK_THRESHOLD) && (playScreen.PixelsWithRGB(CURRENT_TUNIC_R, CURRENT_TUNIC_G, CURRENT_TUNIC_B).size() != 0);
	      if (inSecretCave) {
		RecordSecretCave(screen, mapx, mapy);
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
		{
		  //check if this is level 9
		  ImageHandler lnumber = screen.Crop(REFERENCE_DUNGEON_NINE_XCOOR*SCALE_X, REFERENCE_DUNGEON_NINE_YCOOR*SCALE_Y, REFERENCE_DUNGEON_NINE_WIDTH*SCALE_X, REFERENCE_DUNGEON_NINE_HEIGHT*SCALE_Y).ConvertToBlackAndWhite();
		  if (lnumber.Similarity(dungeonnine) > CAPTURED_DUNGEON_NINE_SIMILARITY_THRESHOLD) {
		    std::pair<int, int> oloc = ZeldaInformationHandler::GetMapLocation();
		    ZeldaInformationHandler::SetSecret(oloc.first, oloc.second, ZeldaInformationHandler::Secrets::DUNGEON_9);
		  }
		}
		{
		  //check for doors
		  //first make sure you're not moving between doors
		  ImageHandler handle = screen.Crop(REFERENCE_DUNGEON_DOOR_HANDLE_XCOOR*SCALE_X, REFERENCE_DUNGEON_DOOR_HANDLE_YCOOR*SCALE_Y, REFERENCE_DUNGEON_DOOR_HANDLE_WIDTH*SCALE_X, REFERENCE_DUNGEON_DOOR_HANDLE_HEIGHT*SCALE_Y);
		  std::tuple<int, int, int> hcolor = handle.MostCommonRGB();
		  handle = handle.FilterRGB(std::get<0>(hcolor), std::get<1>(hcolor), std::get<2>(hcolor));
		  if (handle.Similarity(dungeondoorhandle) > CAPTURED_DUNGEON_DOOR_HANDLE_SIMILARITY_THRESHOLD) {
		    if (dungeondoortransition) {
		      std::pair<int, int> prevloc = ZeldaInformationHandler::GetDungeonLocation();
		      if (prevloc != std::make_pair(mapx, mapy)) {
			dungeondoortransition = false;			
		      }
		    } 
		  }
		  else {
		    dungeondoortransition = true;
		  }
		  if (!dungeondoortransition) {
		    RecordDoors(screen, mapx, mapy);
		    RecordDarkDoors(screen, mapx, mapy);
		  }
		}
		ZeldaInformationHandler::SetDungeonLocation(mapx, mapy, ZeldaInformationHandler::RoomType::UNKNOWN_ROOM);
		{
		  //check for triforce
		  //first see if dead
		  int numhearts = 0;
		  //check for hearts
		  for (int xsep = 0; xsep < 8; ++xsep) {
		    for (int ysep = 0; ysep < 2; ++ysep) {
		      int xcoor = REFERENCE_HEART_XCOOR + xsep * REFERENCE_HEART_X_SEPERATION;
		      int ycoor = REFERENCE_HEART_YCOOR + ysep * REFERENCE_HEART_Y_SEPERATION;
		      ImageHandler heartloc = screen.Crop(xcoor*SCALE_X, ycoor*SCALE_Y, REFERENCE_HEART_WIDTH*SCALE_X, REFERENCE_HEART_HEIGHT*SCALE_Y);
		      double rp = static_cast<double>(heartloc.PixelsWithRGB(HEART_RED_R, HEART_RED_G, HEART_RED_B).size()) / (heartloc.Width() * heartloc.Height());
		      if (rp > CAPTURED_HEART_COLOR_THRESHOLD) {
			numhearts++;
		      }
		    }
		  }
		  bool health = (numhearts > 0);
		  if (health) {
		    //then check for link holding triforce over a black screen
		    ImageHandler playScreen = screen.Crop(REFERENCE_PLAYING_SCREEN_XCOOR*SCALE_X, REFERENCE_PLAYING_SCREEN_YCOOR*SCALE_Y, REFERENCE_PLAYING_SCREEN_WIDTH*SCALE_X, REFERENCE_PLAYING_SCREEN_HEIGHT*SCALE_Y).FilterRGB(BLACK_R, BLACK_G, BLACK_B);
		    //remove edges
		    playScreen = playScreen.Crop(1*SCALE_X, 1*SCALE_Y, playScreen.Width()-2*SCALE_X, playScreen.Height()-2*SCALE_Y);
		    double bp = static_cast<double>(playScreen.PixelsWithRGB(BLACK_R, BLACK_G, BLACK_B).size()) / (playScreen.Width() * playScreen.Height());
		    if (bp > DUNGEON_TRIFORCE_BLACK_THRESHOLD && bp < 1) {
		      std::vector<int> box = BoundingBox(playScreen.PixelsWithRGB(WHITE_R, WHITE_G, WHITE_B));		  
		      if (box[2]/SCALE_X < DUNGEON_TRIFORCE_WIDTH_THRESHOLD && box[3]/SCALE_Y < DUNGEON_TRIFORCE_HEIGHT_THRESHOLD) {
			ZeldaInformationHandler::SetTriforce();
		      }
		    }
		  }
		}
	      }
	    }
	  }
	  if (!foundLink) {
	    //check for items
	    //The minimap cursor does not show in staircases
	    //check if alive	    
	    int numhearts = 0;
	    for (int xsep = 0; xsep < 8; ++xsep) {
	      for (int ysep = 0; ysep < 2; ++ysep) {
		int xcoor = REFERENCE_HEART_XCOOR + xsep * REFERENCE_HEART_X_SEPERATION;
		int ycoor = REFERENCE_HEART_YCOOR + ysep * REFERENCE_HEART_Y_SEPERATION;
		ImageHandler heartloc = screen.Crop(xcoor*SCALE_X, ycoor*SCALE_Y, REFERENCE_HEART_WIDTH*SCALE_X, REFERENCE_HEART_HEIGHT*SCALE_Y);
		double rp = static_cast<double>(heartloc.PixelsWithRGB(HEART_RED_R, HEART_RED_G, HEART_RED_B).size()) / (heartloc.Width() * heartloc.Height());
		//half hearts
		if (rp > CAPTURED_HEART_COLOR_THRESHOLD / 2) {
		  numhearts++;
		}
	      }
	    }
	    bool alive = (numhearts > 0);
	    if (alive) {
	      double maxSim = 0;
	      ZeldaInformationHandler::DungeonItems type;
	      ImageHandler item = screen.Crop(REFERENCE_DUNGEON_ITEM_XCOOR*SCALE_X, REFERENCE_DUNGEON_ITEM_YCOOR*SCALE_Y, REFERENCE_DUNGEON_ITEM_WIDTH*SCALE_X, REFERENCE_DUNGEON_ITEM_HEIGHT*SCALE_Y).FilterRGB(BLACK_R, BLACK_G, BLACK_B);
	      item.SaveAsPPM("tmp.ppm");
	      if (item.Similarity(book) > maxSim) {
		maxSim = item.Similarity(book);
		type = ZeldaInformationHandler::DungeonItems::BOOK;
	      }
	      if (item.Similarity(bow) > maxSim) {
		maxSim = item.Similarity(bow);
		type = ZeldaInformationHandler::DungeonItems::BOW;
	      }
	      if (item.Similarity(ladder) > maxSim) {
		maxSim = item.Similarity(ladder);
		type = ZeldaInformationHandler::DungeonItems::LADDER;
	      }
	      if (item.Similarity(magicalkey) > maxSim) {
		maxSim = item.Similarity(magicalkey);
		type = ZeldaInformationHandler::DungeonItems::MAGICAL_KEY;
	      }
	      if (item.Similarity(raft) > maxSim) {
		maxSim = item.Similarity(raft);
		type = ZeldaInformationHandler::DungeonItems::RAFT;
	      }
	      if (item.Similarity(recorder) > maxSim) {
		maxSim = item.Similarity(recorder);
		type = ZeldaInformationHandler::DungeonItems::RECORDER;
	      }
	      if (item.Similarity(redcandle) > maxSim) {
		maxSim = item.Similarity(redcandle);
		type = ZeldaInformationHandler::DungeonItems::RED_CANDLE;
	      }
	      if (item.Similarity(redring) > maxSim) {
		maxSim = item.Similarity(redring);
		type = ZeldaInformationHandler::DungeonItems::RED_RING;
	      }
	      if (item.Similarity(silverarrow) > maxSim) {
		maxSim = item.Similarity(silverarrow);
		type = ZeldaInformationHandler::DungeonItems::SILVER_ARROW;
	      }
	      //prevent false positive
	      double bp = static_cast<double>(item.PixelsWithRGB(BLACK_R, BLACK_G, BLACK_B).size()) / (item.Width() * item.Height());
	      if (bp > maxSim) {
		maxSim = 0;
		type = ZeldaInformationHandler::DungeonItems::NONE;
	      }
	      if ((1-bp) > maxSim) {
		maxSim = 0;
		type = ZeldaInformationHandler::DungeonItems::NONE;
	      }	      
	      if (maxSim > CAPTURED_DUNGEON_ITEM_SIMILARITY_THRESHOLD) {
		ZeldaInformationHandler::SetItem(type);
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
      {
	int numhearts = 0;
	//check for hearts
	for (int xsep = 0; xsep < 8; ++xsep) {
	  for (int ysep = 0; ysep < 2; ++ysep) {
	    int xcoor = REFERENCE_HEART_XCOOR + xsep * REFERENCE_HEART_X_SEPERATION;
	    int ycoor = REFERENCE_HEART_YCOOR + ysep * REFERENCE_HEART_Y_SEPERATION;
	    ImageHandler heartloc = screen.Crop(xcoor*SCALE_X, ycoor*SCALE_Y, REFERENCE_HEART_WIDTH*SCALE_X, REFERENCE_HEART_HEIGHT*SCALE_Y);
	    double bp = static_cast<double>(heartloc.PixelsWithRGB(BLACK_R, BLACK_G, BLACK_B).size()) / (heartloc.Width() * heartloc.Height());
	    if (!(bp > CAPTURED_HEART_COLOR_THRESHOLD)) {
	      numhearts++;
	    }
	  }
	}
	ZeldaInformationHandler::SetHearts(numhearts);
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

void ZeldaImageProcessor::RecordDoors(ImageHandler& screen, int mapx, int mapy) {
  {
    ImageHandler dl = screen.Crop(REFERENCE_LEFT_DUNGEON_DOOR_XCOOR*SCALE_X, REFERENCE_LEFT_DUNGEON_DOOR_YCOOR*SCALE_Y, REFERENCE_LEFT_DUNGEON_DOOR_WIDTH*SCALE_X, REFERENCE_LEFT_DUNGEON_DOOR_HEIGHT*SCALE_Y);
    {
      ImageHandler dld = dl.FilterRGB(BLACK_R, BLACK_G, BLACK_B);
      ImageHandler zeldaDLD = dungeonld;
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
      ImageHandler zeldaDLK = dungeonlk;
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
      ImageHandler zeldaDLK = dungeonls;
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
      ImageHandler zeldaDLD = dungeonlb;
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
      ImageHandler zeldaDLD = dungeonrd;
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
      ImageHandler zeldaDLK = dungeonrk;
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
      ImageHandler zeldaDLK = dungeonrs;
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
      ImageHandler zeldaDLD = dungeonrb;
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
      ImageHandler zeldaDLD = dungeontd;
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
      ImageHandler zeldaDLK = dungeontk;
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
      ImageHandler zeldaDLK = dungeonts;
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
      ImageHandler zeldaDLD = dungeontb;
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
      ImageHandler zeldaDLD = dungeonbd;
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
      ImageHandler zeldaDLK = dungeonbk;
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
      ImageHandler zeldaDLK = dungeonbs;
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
    ImageHandler zeldaDLD = dungeonbd;
    double simDLD = std::min(zeldaDLD.Similarity(dld), dld.Similarity(zeldaDLD));
    if (simDLD > CAPTURED_BOTTOM_BOMB_DOOR_SIMILARITY_THRESHOLD) {
      ZeldaInformationHandler::SetDungeonLocation(mapx, mapy+1, ZeldaInformationHandler::RoomType::UNSEEN_ROOM);
      ZeldaInformationHandler::SetDungeonDoor(mapx, mapy, mapx, mapy+1, ZeldaInformationHandler::DoorType::BOMB);
    }
  }
}

void ZeldaImageProcessor::RecordDarkDoors(ImageHandler& screen, int mapx, int mapy) {
  {
    ImageHandler dl = screen.Crop(REFERENCE_LEFT_DUNGEON_DOOR_XCOOR*SCALE_X, REFERENCE_LEFT_DUNGEON_DOOR_YCOOR*SCALE_Y, REFERENCE_LEFT_DUNGEON_DOOR_WIDTH*SCALE_X, REFERENCE_LEFT_DUNGEON_DOOR_HEIGHT*SCALE_Y);
    ZeldaInformationHandler::DoorType bestFit;
    double bestSim = 0;
    {
      ImageHandler dld = dl.FilterRGB(BLACK_R, BLACK_G, BLACK_B);
      ImageHandler zeldaDLD = dungeondld;
      double simDLD = dld.Similarity(zeldaDLD);
      if (simDLD > bestSim) {
	bestSim = simDLD;
	bestFit = ZeldaInformationHandler::DoorType::OPEN;
      }
    }
    {
      ImageHandler dld = dl.FilterRGB(BLACK_R, BLACK_G, BLACK_B);
      ImageHandler zeldaDLD = dungeondlk;
      double simDLD = dld.Similarity(zeldaDLD);
      if (simDLD > bestSim) {
	bestSim = simDLD;
	bestFit = ZeldaInformationHandler::DoorType::KEY;
      }
    }
    {
      ImageHandler dld = dl.FilterRGB(BLACK_R, BLACK_G, BLACK_B);
      ImageHandler zeldaDLD = dungeondls;
      double simDLD = dld.Similarity(zeldaDLD);
      if (simDLD > bestSim) {
	bestSim = simDLD;
	bestFit = ZeldaInformationHandler::DoorType::SHUTTER;
      }
    }
    {
      ImageHandler dld = dl.FilterRGB(BLACK_R, BLACK_G, BLACK_B);
      ImageHandler zeldaDLD = dungeondlb;
      double simDLD = dld.Similarity(zeldaDLD);
      if (simDLD > bestSim) {
	bestSim = simDLD;
	bestFit = ZeldaInformationHandler::DoorType::BOMB;
      }
    }
    {
      //prevent false positives
      double bp = static_cast<double>(dl.PixelsWithRGB(BLACK_R, BLACK_G, BLACK_B).size()) / (dl.Width() * dl.Height());
      if (bp == 1) {
	bestSim = 0;
      }
    }
    if (bestSim > CAPTURED_DUNGEON_DARK_ROOM_SIMILARITY_THRESHOLD &&
	!((bestSim < CAPTURED_DUNGEON_DARK_BOMB_ROOM_SIMILARITY_THRESHOLD) && (bestFit == ZeldaInformationHandler::DoorType::BOMB))) {
      ZeldaInformationHandler::SetDungeonLocation(mapx-1, mapy, ZeldaInformationHandler::RoomType::UNSEEN_ROOM);	
      ZeldaInformationHandler::SetDungeonDoor(mapx, mapy, mapx-1, mapy, bestFit);
    }
  }
  {
    ImageHandler dl = screen.Crop(REFERENCE_RIGHT_DUNGEON_DOOR_XCOOR*SCALE_X, REFERENCE_RIGHT_DUNGEON_DOOR_YCOOR*SCALE_Y, REFERENCE_RIGHT_DUNGEON_DOOR_WIDTH*SCALE_X, REFERENCE_RIGHT_DUNGEON_DOOR_HEIGHT*SCALE_Y);
    ZeldaInformationHandler::DoorType bestFit;
    double bestSim = 0;
    {
      ImageHandler dld = dl.FilterRGB(BLACK_R, BLACK_G, BLACK_B);
      ImageHandler zeldaDLD = dungeondrd;
      double simDLD = dld.Similarity(zeldaDLD);
      if (simDLD > bestSim) {
	bestSim = simDLD;
	bestFit = ZeldaInformationHandler::DoorType::OPEN;
      }
    }
    {
      ImageHandler dld = dl.FilterRGB(BLACK_R, BLACK_G, BLACK_B);
      ImageHandler zeldaDLD = dungeondrk;
      double simDLD = dld.Similarity(zeldaDLD);
      if (simDLD > bestSim) {
	bestSim = simDLD;
	bestFit = ZeldaInformationHandler::DoorType::KEY;
      }
    }
    {
      ImageHandler dld = dl.FilterRGB(BLACK_R, BLACK_G, BLACK_B);
      ImageHandler zeldaDLD = dungeondrs;
      double simDLD = dld.Similarity(zeldaDLD);
      if (simDLD > bestSim) {
	bestSim = simDLD;
	bestFit = ZeldaInformationHandler::DoorType::SHUTTER;
      }
    }
    {
      ImageHandler dld = dl.FilterRGB(BLACK_R, BLACK_G, BLACK_B);
      ImageHandler zeldaDLD = dungeondrb;
      double simDLD = dld.Similarity(zeldaDLD);
      if (simDLD > bestSim) {
	bestSim = simDLD;
	bestFit = ZeldaInformationHandler::DoorType::BOMB;
      }
    }
    {
      //prevent false positives
      double bp = static_cast<double>(dl.PixelsWithRGB(BLACK_R, BLACK_G, BLACK_B).size()) / (dl.Width() * dl.Height());
      if (bp == 1) {
	bestSim = 0;
      }
    }    
    if (bestSim > CAPTURED_DUNGEON_DARK_ROOM_SIMILARITY_THRESHOLD &&
	!((bestSim < CAPTURED_DUNGEON_DARK_BOMB_ROOM_SIMILARITY_THRESHOLD) && (bestFit == ZeldaInformationHandler::DoorType::BOMB))) {
      ZeldaInformationHandler::SetDungeonLocation(mapx+1, mapy, ZeldaInformationHandler::RoomType::UNSEEN_ROOM);	
      ZeldaInformationHandler::SetDungeonDoor(mapx, mapy, mapx+1, mapy, bestFit);
    }
  }
  {
    ImageHandler dl = screen.Crop(REFERENCE_TOP_DUNGEON_DOOR_XCOOR*SCALE_X, REFERENCE_TOP_DUNGEON_DOOR_YCOOR*SCALE_Y, REFERENCE_TOP_DUNGEON_DOOR_WIDTH*SCALE_X, REFERENCE_TOP_DUNGEON_DOOR_HEIGHT*SCALE_Y);
    ZeldaInformationHandler::DoorType bestFit;
    double bestSim = 0;
    {
      ImageHandler dld = dl.FilterRGB(BLACK_R, BLACK_G, BLACK_B);
      ImageHandler zeldaDLD = dungeondtd;
      double simDLD = dld.Similarity(zeldaDLD);
      if (simDLD > bestSim) {
	bestSim = simDLD;
	bestFit = ZeldaInformationHandler::DoorType::OPEN;
      }
    }
    {
      ImageHandler dld = dl.FilterRGB(BLACK_R, BLACK_G, BLACK_B);
      ImageHandler zeldaDLD = dungeondtk;
      double simDLD = dld.Similarity(zeldaDLD);
      if (simDLD > bestSim) {
	bestSim = simDLD;
	bestFit = ZeldaInformationHandler::DoorType::KEY;
      }
    }
    {
      ImageHandler dld = dl.FilterRGB(BLACK_R, BLACK_G, BLACK_B);
      ImageHandler zeldaDLD = dungeondts;
      double simDLD = dld.Similarity(zeldaDLD);
      if (simDLD > bestSim) {
	bestSim = simDLD;
	bestFit = ZeldaInformationHandler::DoorType::SHUTTER;
      }
    }
    {
      ImageHandler dld = dl.FilterRGB(BLACK_R, BLACK_G, BLACK_B);
      ImageHandler zeldaDLD = dungeondtb;
      double simDLD = dld.Similarity(zeldaDLD);
      if (simDLD > bestSim) {
	bestSim = simDLD;
	bestFit = ZeldaInformationHandler::DoorType::BOMB;
      }
    }
    {
      //prevent false positives
      double bp = static_cast<double>(dl.PixelsWithRGB(BLACK_R, BLACK_G, BLACK_B).size()) / (dl.Width() * dl.Height());
      if (bp == 1) {
	bestSim = 0;
      }
    }    
    if (bestSim > CAPTURED_DUNGEON_DARK_ROOM_SIMILARITY_THRESHOLD &&
	!((bestSim < CAPTURED_DUNGEON_DARK_BOMB_ROOM_SIMILARITY_THRESHOLD) && (bestFit == ZeldaInformationHandler::DoorType::BOMB))) {
      ZeldaInformationHandler::SetDungeonLocation(mapx, mapy-1, ZeldaInformationHandler::RoomType::UNSEEN_ROOM);	
      ZeldaInformationHandler::SetDungeonDoor(mapx, mapy, mapx, mapy-1, bestFit);
    }
  }
  {
    ImageHandler dl = screen.Crop(REFERENCE_BOTTOM_DUNGEON_DOOR_XCOOR*SCALE_X, REFERENCE_BOTTOM_DUNGEON_DOOR_YCOOR*SCALE_Y, REFERENCE_BOTTOM_DUNGEON_DOOR_WIDTH*SCALE_X, REFERENCE_BOTTOM_DUNGEON_DOOR_HEIGHT*SCALE_Y);
    ZeldaInformationHandler::DoorType bestFit;
    double bestSim = 0;
    {
      ImageHandler dld = dl.FilterRGB(BLACK_R, BLACK_G, BLACK_B);
      ImageHandler zeldaDLD = dungeondbd;
      double simDLD = dld.Similarity(zeldaDLD);
      if (simDLD > bestSim) {
	bestSim = simDLD;
	bestFit = ZeldaInformationHandler::DoorType::OPEN;
      }
    }
    {
      ImageHandler dld = dl.FilterRGB(BLACK_R, BLACK_G, BLACK_B);
      ImageHandler zeldaDLD = dungeondbk;
      double simDLD = dld.Similarity(zeldaDLD);
      if (simDLD > bestSim) {
	bestSim = simDLD;
	bestFit = ZeldaInformationHandler::DoorType::KEY;
      }
    }
    {
      ImageHandler dld = dl.FilterRGB(BLACK_R, BLACK_G, BLACK_B);
      ImageHandler zeldaDLD = dungeondbs;
      double simDLD = dld.Similarity(zeldaDLD);
      if (simDLD > bestSim) {
	bestSim = simDLD;
	bestFit = ZeldaInformationHandler::DoorType::SHUTTER;
      }
    }
    {
      ImageHandler dld = dl.FilterRGB(BLACK_R, BLACK_G, BLACK_B);
      ImageHandler zeldaDLD = dungeondbb;
      double simDLD = dld.Similarity(zeldaDLD);
      if (simDLD > bestSim) {
	bestSim = simDLD;
	bestFit = ZeldaInformationHandler::DoorType::BOMB;
      }
    }
    {
      //prevent false positives
      double bp = static_cast<double>(dl.PixelsWithRGB(BLACK_R, BLACK_G, BLACK_B).size()) / (dl.Width() * dl.Height());
      if (bp == 1) {
	bestSim = 0;
      }
    }
    if (bestSim > CAPTURED_DUNGEON_DARK_ROOM_SIMILARITY_THRESHOLD &&
	!((bestSim < CAPTURED_DUNGEON_DARK_BOMB_ROOM_SIMILARITY_THRESHOLD) && (bestFit == ZeldaInformationHandler::DoorType::BOMB))) {
      while (true) {

      }
      ZeldaInformationHandler::SetDungeonLocation(mapx, mapy+1, ZeldaInformationHandler::RoomType::UNSEEN_ROOM);	
      ZeldaInformationHandler::SetDungeonDoor(mapx, mapy, mapx, mapy+1, bestFit);
    }
  }
}

void ZeldaImageProcessor::RecordSecretCave(ImageHandler& screen, int mapx, int mapy) {
  bool foundSecret = false;
  //check if in a potion shop before showing letter. Check this case first because it is the easiest to mislabel
  {
    ImageHandler item = screen.Crop(REFERENCE_POTION_SHOP_TEXT_XCOOR*SCALE_X, REFERENCE_POTION_SHOP_TEXT_YCOOR*SCALE_Y, REFERENCE_POTION_SHOP_TEXT_WIDTH*SCALE_X, REFERENCE_POTION_SHOP_TEXT_HEIGHT*SCALE_Y);
    //If letter has not been shown, then potion shop will have no text
    ZeldaInformationHandler::Secrets prev = ZeldaInformationHandler::GetSecret(mapx, mapy);
    if ((prev == ZeldaInformationHandler::Secrets::UNKNOWN_CAVE) || (prev == ZeldaInformationHandler::Secrets::UNEXPLORED) || (prev == ZeldaInformationHandler::Secrets::PRE_POTION_SHOP)) {
      if (item.PixelsWithRGB(WHITE_R, WHITE_G, WHITE_B).size() == 0) {
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
    ImageHandler road = screen.Crop(REFERENCE_ANYROAD_XCOOR*SCALE_X, REFERENCE_ANYROAD_YCOOR*SCALE_Y, REFERENCE_ANYROAD_WIDTH*SCALE_X, REFERENCE_ANYROAD_HEIGHT*SCALE_Y).FilterRGB(WHITE_R, WHITE_G, WHITE_B);
    if (road.Similarity(anyroad) > CAPTURED_ANYROAD_SIMILARITY_THRESHOLD) {
      ZeldaInformationHandler::SetSecret(mapx, mapy, ZeldaInformationHandler::Secrets::ANYROAD);
      foundSecret = true;
    }
  }
  //check if in sword cave
  {
    bool foundWhiteSword = false;
    {
      ImageHandler sword = screen.Crop(REFERENCE_WHITE_SWORD_XCOOR*SCALE_X, REFERENCE_WHITE_SWORD_YCOOR*SCALE_Y, REFERENCE_WHITE_SWORD_WIDTH*SCALE_X, REFERENCE_WHITE_SWORD_HEIGHT*SCALE_Y).FilterRGB(WHITE_R, WHITE_G, WHITE_B);
      if (sword.Similarity(whitesword) > CAPTURED_SWORD_SIMILARITY_THRESHOLD) {
	ZeldaInformationHandler::SetSecret(mapx, mapy, ZeldaInformationHandler::Secrets::WHITE_SWORD);
	foundSecret = true;
	foundWhiteSword = true;
      }
    }
    {
      if (!foundWhiteSword) {
	ImageHandler sword = screen.Crop(REFERENCE_MAGICAL_SWORD_XCOOR*SCALE_X, REFERENCE_MAGICAL_SWORD_YCOOR*SCALE_Y, REFERENCE_MAGICAL_SWORD_WIDTH*SCALE_X, REFERENCE_MAGICAL_SWORD_HEIGHT*SCALE_Y).FilterRGB(WHITE_R, WHITE_G, WHITE_B);
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
    ImageHandler item = screen.Crop(REFERENCE_SHOP_LEFT_ITEM_XCOOR*SCALE_X, REFERENCE_SHOP_LEFT_ITEM_YCOOR*SCALE_Y, REFERENCE_SHOP_LEFT_ITEM_WIDTH*SCALE_X, REFERENCE_SHOP_LEFT_ITEM_HEIGHT*SCALE_Y).FilterRGB(BLACK_R, BLACK_G, BLACK_B);
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
    ImageHandler item = screen.Crop(REFERENCE_SHOP_MIDDLE_ITEM_XCOOR*SCALE_X, REFERENCE_SHOP_MIDDLE_ITEM_YCOOR*SCALE_Y, REFERENCE_SHOP_MIDDLE_ITEM_WIDTH*SCALE_X, REFERENCE_SHOP_MIDDLE_ITEM_HEIGHT*SCALE_Y).FilterRGB(BLACK_R, BLACK_G, BLACK_B);
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
    ImageHandler item = screen.Crop(REFERENCE_SHOP_RIGHT_ITEM_XCOOR*SCALE_X, REFERENCE_SHOP_RIGHT_ITEM_YCOOR*SCALE_Y, REFERENCE_SHOP_RIGHT_ITEM_WIDTH*SCALE_X, REFERENCE_SHOP_RIGHT_ITEM_HEIGHT*SCALE_Y).FilterRGB(BLACK_R, BLACK_G, BLACK_B);
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
