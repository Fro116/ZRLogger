#ifndef StartupState_1480118156631006
#define StartupState_1480118156631006

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "GameState.h"
#include "Drawable.h"
#include "Updatable.h"
#include "Bindable.h"
#include "Clickable.h"

#include "GameDriver.h"

class StartupState : public GameState {
public:
  StartupState(std::shared_ptr<GameDriver> gameDriver, std::string vertexShaderPath, std::string fragmentShaderPath);
  
  void BindObject(std::shared_ptr<Bindable> object);
  void UnbindObject(std::shared_ptr<Bindable> object);
  
  void EnterFocus() override;
  void LeaveFocus() override;
  
  void Update() override;
  void Draw(double time) override;
  void ClickHandler(int button, int action, int mods);
  
  static void ClickHandler(GLFWwindow* window, int button, int action, int mods);
 private:
  GLFWwindow* window;
  GLuint shaders;
  GLuint transformID;
  std::vector<std::shared_ptr<Bindable>> bindables;
  std::vector<std::shared_ptr<Drawable>> drawables;
  std::vector<std::shared_ptr<Updatable>> updatables;
  std::vector<std::shared_ptr<Clickable>> clickables;  
  std::shared_ptr<GameDriver> driver;
};

#endif
