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

#include <fstream>
#include "OpenGLUtility.h"

#include "GameDriver.h"
#include "HeartsState.h"

void GraphicsThread() {
  double fps = 60;
  double npf = 1e9 / fps;
  long nanosPerFrame = static_cast<long>(npf);

  
  GameDriver driver("Hearts", 800, 600, nanosPerFrame,5);  
  GameEngine& engine = driver.Engine();
  auto state = HeartsState::CreateInstance(engine.Window(), "Abstract Graphics/Shaders/TextureShader.vertexshader", "Abstract Graphics/Shaders/TextureShader.fragmentshader");
  engine.PushState(state);
  driver.Run();
}

int main(int argc, const char * argv[]) {
  GraphicsThread();
  return 0;
}



