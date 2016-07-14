//
//  OverworldState.h
//  Overworld
//
//  Created by Kundan Chintamaneni on 2/21/15.
//  Copyright (c) 2015 Kundan Chintamaneni. All rights reserved.
//

#ifndef __Overworld__OverworldState__
#define __Overworld__OverworldState__

#include <memory>
#include <vector>
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "GameState.h"
#include "Drawable.h"
#include "Updatable.h"
#include "Bindable.h"

#include "GameDriver.h"

class OverworldState : public GameState {
public:
  OverworldState(std::shared_ptr<GameDriver> gameDriver, std::string vertexShaderPath, std::string fragmentShaderPath);
  ~OverworldState();
  
  void BindObject(std::shared_ptr<Bindable> object);
  void UnbindObject(std::shared_ptr<Bindable> object);
  
  void EnterFocus() override;
  void LeaveFocus() override;
  
  void Update() override;
  void Draw(double time) override;
 private:
  GLFWwindow* window;
  GLuint shaders;
  GLuint transformID;
  std::vector<std::shared_ptr<Bindable>> bindables;
  std::vector<std::shared_ptr<Drawable>> drawables;
  std::vector<std::shared_ptr<Updatable>> updatables;
  std::shared_ptr<GameDriver> driver;
};

#endif /* defined(__Overworld__OverworldState__) */
