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
#include "OverworldState.h"
#include "ZeldaImageProcessor.h"

void GraphicsThread() {
  double fps = 60;
  double npf = 1e9 / fps;
  long nanosPerFrame = static_cast<long>(npf);
  
  GameDriver driver("ZRLogger", 800, 300, nanosPerFrame,5);  
  GameEngine& engine = driver.Engine();
  auto state = OverworldState::CreateInstance(engine.Window(), "Abstract Graphics/Shaders/TextureShader.vertexshader", "Abstract Graphics/Shaders/TextureShader.fragmentshader");
  engine.PushState(state);
  driver.Run();
}

void ProcessingThread() {
  ZeldaImageProcessor zir;
  zir.PrintDebugData();
  while (true) {
    zir.UpdateData();
  }
}

int main(int argc, const char * argv[]) {
  std::thread processing(ProcessingThread);
  GraphicsThread();
  processing.join();
  return 0;
}



