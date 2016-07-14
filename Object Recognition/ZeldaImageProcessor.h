#ifndef ZeldaImageProcessor_1468349491158922
#define ZeldaImageProcessor_1468349491158922

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <memory>
#include <cassert>
#include <limits>
#include <chrono>
#include <set>
#include <cstdlib>
#include <tuple>

#include "ImageHandler.h"

class ZeldaImageProcessor {
 public:
  ZeldaImageProcessor();
  void UpdateData();
  void PrintDebugData();
 private:
  void FindZeldaScreen();
  std::vector<int> BoundingBox(std::vector<std::pair<int,int>> coordinates); //returns topleftx, toplefty, width, height
  std::vector<std::vector<std::pair<int,int>>> ConnectedComponents(std::vector<std::pair<int,int>> coordinates);

  ImageHandler GetScreen(); //returns a cropped screenshot that captures the zelda playing screen
  
  int topleftx;
  int toplefty;
  int screenWidth;
  int screenHeight;

  int WHITE_R = 255;
  int WHITE_G = 255;
  int WHITE_B = 255;

  int BLACK_R = 0;
  int BLACK_G = 0;
  int BLACK_B = 0;

  int REFERENCE_SCREEN_WIDTH;
  int REFERENCE_SCREEN_HEIGHT;

  int REFERENCE_REGISTRATION_SCREEN_C_XCOOR;
  int REFERENCE_REGISTRATION_SCREEN_C_YCOOR;
  int REFERENCE_REGISTRATION_SCREEN_G_XCOOR;
  int REFERENCE_REGISTRATION_SCREEN_G_YCOOR;

  int CAPTURED_REGISTRATION_SCREEN_WIDTH;
  int CAPTURED_REGISTRATION_SCREEN_HEIGHT;
  int CAPTURED_REGISTRATION_SCREEN_XCOOR;
  int CAPTURED_REGISTRATION_SCREEN_YCOOR;
  double CAPTURED_REGISTRATION_SCREEN_SIMILARITY_THRESHOLD = 0.50;

  double SCALE_X;
  double SCALE_Y;

  int REFERENCE_OVERWORLD_MINIMAP_XCOOR = 16;
  int REFERENCE_OVERWORLD_MINIMAP_YCOOR = 16;
  int REFERENCE_OVERWORLD_MINIMAP_WIDTH = 64;
  int REFERENCE_OVERWORLD_MINIMAP_HEIGHT = 32;

  bool INITIALIZED_MINIMAP_GRAY = false;
  bool INITIALIZED_TUNIC_GREEN = false;
  bool INITIALIZED_CURRENT_TUNIC = false;
  
  int TUNIC_GREEN_R;
  int TUNIC_GREEN_G;
  int TUNIC_GREEN_B;

  int MINIMAP_GRAY_R;
  int MINIMAP_GRAY_G;
  int MINIMAP_GRAY_B;

  int CURRENT_TUNIC_R;
  int CURRENT_TUNIC_G;
  int CURRENT_TUNIC_B;
};

#endif
