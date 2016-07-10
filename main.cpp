//
//  main.cpp
//  Hearts
//
//  Created by Kundan Chintamaneni on 2/7/15.
//  Copyright (c) 2015 Kundan Chintamaneni. All righwts reserved.
//

#include <iostream>
#include <memory>
#include <thread>

#include "HeartsDriver.h"

void GraphicsThread() {
    double fps = 60;
    double npf = 1e9 / fps;
    long nanosPerFrame = static_cast<long>(npf);
    
    HeartsDriver driver(nanosPerFrame,5);
    driver.Run();
    std::cout << "DONE" << std::endl;
}

int main(int argc, const char * argv[]) {    
    GraphicsThread();
    return 0;
}



