//
//  HeartsDriver.cpp
//  Hearts
//
//  Created by Kundan Chintamaneni on 2/21/15.
//  Copyright (c) 2015 Kundan Chintamaneni. All rights reserved.
//

#include "GameDriver.h"

#include <iostream>
#include <memory>

#include "GameEngine.h"

GameDriver::GameDriver(std::string title, int width, int height, long nanosPerFrame, int maxSkippedDrawSteps) : nanosPerFrame(nanosPerFrame), maxSkippedDrawSteps(maxSkippedDrawSteps), game(title, width, height), offset(0), skippedDrawSteps(0) {
    time = std::chrono::steady_clock::now();
}

GameEngine& GameDriver::Engine() {
  return game;
}

void GameDriver::Run() {
    time = std::chrono::steady_clock::now();
    while (game.IsRunning()) {
        auto curTime = std::chrono::steady_clock::now();
        auto diffTime = std::chrono::duration_cast<std::chrono::nanoseconds>(curTime - time);
        long difference = diffTime.count();
        if ((difference + offset >= nanosPerFrame) && (skippedDrawSteps <= maxSkippedDrawSteps)) {
            offset = difference + offset - nanosPerFrame;
            time = curTime;
	    game.Update();
            ++skippedDrawSteps;
        }
        else {
            skippedDrawSteps = 0;
            game.Draw(difference + offset);
        }
    }
}
