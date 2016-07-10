//
//  GameEngine.h
//  Hearts
//
//  Created by Kundan Chintamaneni on 2/21/15.
//  Copyright (c) 2015 Kundan Chintamaneni. All rights reserved.
//

#ifndef __Hearts__GameEngine__
#define __Hearts__GameEngine__

#include "GameState.h"

#include <memory>
#include <stack>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>

class GameEngine {
public:
  GameEngine(std::string title, int screenWidth, int screenHeight);
  void PushState(std::shared_ptr<GameState>  state);
  std::shared_ptr<GameState>  PopState();
  
  void Update();
  void Draw(double time);
  
  bool IsRunning();
  void Quit();
  
  GLFWwindow* Window();
private:
  std::stack<std::shared_ptr<GameState>> states;
  bool isRunning;
  GLFWwindow* window;
};

#endif /* defined(__Hearts__GameEngine__) */
