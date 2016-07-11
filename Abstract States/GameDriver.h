//
//  HeartsDriver.h
//  Hearts
//
//  Created by Kundan Chintamaneni on 2/21/15.
//  Copyright (c) 2015 Kundan Chintamaneni. All rights reserved.
//

//TODO Make into abstact base class

#ifndef __Hearts__GameDriver__
#define __Hearts__GameDriver__

#include <chrono>

#include "GameEngine.h"

class GameDriver {
public:
  GameDriver(std::string title, int width, int height, long nanosPerFrame, int maxSkippedDrawSteps);
  void Run();
  GameEngine& Engine();
 private:
  long nanosPerFrame;
  long offset;
  int maxSkippedDrawSteps;
  int skippedDrawSteps;
  std::chrono::steady_clock::time_point time;
  GameEngine game;
};

#endif /* defined(__Hearts__GameDriver__) */
