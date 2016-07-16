#include "DungeonState.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include "DungeonSelector.h"
#include "DungeonMarker.h"
#include "DungeonDoor.h"
#include "OpenGLUtility.h"
#include "ZeldaInformationHandler.h"

DungeonState::DungeonState(std::shared_ptr<GameDriver> gameDriver, std::string vertexShaderPath, std::string fragmentShaderPath) : window(gameDriver->Engine().Window()), drawables(), updatables() {
  driver = gameDriver;
  shaders = OpenGLUtility::LoadShaders(vertexShaderPath.c_str(), fragmentShaderPath.c_str());
  transformID = glGetUniformLocation(shaders, "transform");
  for (int row = 0; row < 8; ++row) {
    for (int col = 0; col < 8; ++col) {
      double px = 100;
      double py = px*336.0/1024.0;
      OpenGLRectangle* rdoor =  new DungeonDoor(col, 7-row, col+1, 7-row);
      double rx = ((px*col + px/2) + (px*(col+1) + px/2)) / 2;
      double ry = py*row+py/2;
      rdoor->MoveTo(glm::vec3(rx, ry,0.000));                 
      std::shared_ptr<Bindable> rdoorObject(rdoor);
      BindObject(rdoorObject);
      OpenGLRectangle* ddoor =  new DungeonDoor(col, 7-row, col, 7-(row+1));
      double dx = (px*col + px/2);
      double dy = ((py*row+py/2) + (py*(row+1)+py/2)) / 2;
      ddoor->MoveTo(glm::vec3(dx, dy,0.000));                 
      std::shared_ptr<Bindable> ddoorObject(ddoor);
      BindObject(ddoorObject);      
    }                                                                        
  }
  for (int row = 0; row < 8; ++row) {
    for (int col = 0; col < 8; ++col) {
      double px = 100;
      double py = px*336.0/1024.0;
      OpenGLRectangle* marker =  new DungeonMarker(col, 7-row);
      marker->MoveTo(glm::vec3(px*col + px/2, py*row+py/2,0.001));                 
      std::shared_ptr<Bindable> markerObject(marker);
      BindObject(markerObject);
    }                                                                        
  }
  OpenGLRectangle* selector =  new DungeonSelector();
  std::shared_ptr<Bindable> selObject(selector);
  BindObject(selObject);
}

void DungeonState::BindObject(std::shared_ptr<Bindable> object) {
    bindables.push_back(object);
    
    std::shared_ptr<Updatable> updatable = std::dynamic_pointer_cast<Updatable>(object);
    if (updatable) {
        updatables.push_back(updatable);
    }
    
    std::shared_ptr<Drawable> drawable = std::dynamic_pointer_cast<Drawable>(object);
    if (drawable) {
        drawables.push_back(drawable);
    }
    
    object->Bind(this);
}

void DungeonState::UnbindObject(std::shared_ptr<Bindable> object) {    
    std::shared_ptr<Updatable> updatable = std::dynamic_pointer_cast<Updatable>(object);
    if (updatable) {
        auto it = std::find(updatables.begin(), updatables.end(), updatable);
        if (it != updatables.end()) {
            updatables.erase(it);
        }
    }
    
    std::shared_ptr<Drawable> drawable = std::dynamic_pointer_cast<Drawable>(object);
    if (drawable) {
        auto it = std::find(drawables.begin(), drawables.end(), drawable);
        if (it != drawables.end()) {
            drawables.erase(it);
        }
    }
    
    object->Unbind(this);
}

void DungeonState::EnterFocus() {

}

void DungeonState::LeaveFocus() {

}

void DungeonState::Update() {
  for (std::shared_ptr<Updatable> object : updatables) {
    object->Update();
  }
  if (!ZeldaInformationHandler::GetIsInDungeon()) {
    auto ds = driver->Engine().PopState();
    auto os = driver->Engine().PopState();
    driver->Engine().PushState(ds);
    driver->Engine().PushState(os);
  }
}

void DungeonState::Draw(double time) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glUseProgram(shaders);

  float x = 800;
  float y = 400;
  float z = y/2;
  float aspectRatio = x/y;
  float fov = glm::pi<float>()/2;
  glm::mat4 Projection = glm::perspective(fov, aspectRatio, 10.0f, z+1.0f);
  glm::mat4 View       = glm::lookAt(glm::vec3(x/2,y/2,z), glm::vec3(x/2,y/2,0), glm::vec3(0,1,0));
  glm::mat4 PV       = Projection * View;
    
  for (auto object : drawables) {
    object->SetCameraTransform(PV);
    object->SetTransformID(transformID);
    object->Draw(time);
  }
    
  glfwSwapBuffers(window);
  glUseProgram(0);    
  glfwPollEvents();
}
