//
//  main.cpp
//  Hearts
//
//  Created by Kundan Chintamaneni on 2/7/15.
//  Copyright (c) 2015 Kundan Chintamaneni. All righwts reserved.
//

#define NOMINMAX
#include <windows.h>
#include <iostream>
#include <memory>
#include <thread>

#include <fstream>
#include "OpenGLUtility.h"

#include "ZRDriver.h"
#include "OverworldState.h"
#include "DungeonState.h"
#include "StartupState.h"
#include "ZeldaImageProcessor.h"
#include "ZeldaInformationHandler.h"

void GraphicsThread() {
  double fps = 60;
  double npf = 1e9 / fps;
  long nanosPerFrame = static_cast<long>(npf);

  std::shared_ptr<GameDriver> driver = std::make_shared<ZRDriver>("ZRLogger", 600, 300, nanosPerFrame,5);
  auto dstate = std::make_shared<DungeonState>(driver, "Abstract Graphics/Shaders/TextureShader.vertexshader", "Abstract Graphics/Shaders/TextureShader.fragmentshader");
  auto ostate = std::make_shared<OverworldState>(driver, "Abstract Graphics/Shaders/TextureShader.vertexshader", "Abstract Graphics/Shaders/TextureShader.fragmentshader");
  auto sstate = std::make_shared<StartupState>(driver, "Abstract Graphics/Shaders/TextureShader.vertexshader", "Abstract Graphics/Shaders/TextureShader.fragmentshader");  
  driver->Engine().PushState(dstate);
  driver->Engine().PushState(ostate);
  driver->Engine().PushState(sstate);    
  driver->Run();
}

void ProcessingThread() {
  ZeldaImageProcessor zir;
  while (ZeldaInformationHandler::GetIsRunning()) {
    zir.UpdateData();
  }
}

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow) {
  std::thread processing(ProcessingThread);
  GraphicsThread();
  processing.join();
  return 0;
}
