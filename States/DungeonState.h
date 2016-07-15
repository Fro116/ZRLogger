#ifndef DungeonState_1468541790679911
#define DungeonState_1468541790679911

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
#include <map>
#include <thread>
#include <mutex>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "GameState.h"
#include "Drawable.h"
#include "Updatable.h"
#include "Bindable.h"

#include "GameDriver.h"

class DungeonState : public GameState {
 public:
  DungeonState(std::shared_ptr<GameDriver> gameDriver, std::string vertexShaderPath, std::string fragmentShaderPath);
  
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
 private:
};

#endif
