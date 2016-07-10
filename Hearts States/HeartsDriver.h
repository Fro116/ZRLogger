//
//  HeartsDriver.h
//  Hearts
//
//  Created by Kundan Chintamaneni on 2/21/15.
//  Copyright (c) 2015 Kundan Chintamaneni. All rights reserved.
//

//TODO Make into abstact base class

#ifndef __Hearts__HeartsDriver__
#define __Hearts__HeartsDriver__

#include <chrono>

#include "GameEngine.h"
#include "HeartsState.h"

class HeartsDriver {
public:
    HeartsDriver(long nanosPerFrame, int maxSkippedDrawSteps);
    void Run();
private:
    long nanosPerFrame;
    long offset;
    int maxSkippedDrawSteps;
    int skippedDrawSteps;
    std::chrono::steady_clock::time_point time;
    GameEngine game;
};

#endif /* defined(__Hearts__HeartsDriver__) */
