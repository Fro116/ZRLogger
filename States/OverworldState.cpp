//
//  OverworldState.cpp
//  Overworld
//
//  Created by Kundan Chintamaneni on 2/21/15.
//  Copyright (c) 2015 Kundan Chintamaneni. All rights reserved.
//

#include "OverworldState.h"

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include "OpenGLRectangle.h"
#include "OverworldSelector.h"
#include "OverworldMarker.h"
#include "OverworldMap.h"
#include "OpenGLUtility.h"
#include "ZeldaFindingScreen.h"
#include "ZeldaInformationHandler.h"
#include "TriforceMarker.h"
#include "HeartMarker.h"
#include "ItemMarker.h"
#include "BonusCaveMarker.h"

OverworldState::OverworldState(std::shared_ptr<GameDriver> gameDriver, std::string vertexShaderPath, std::string fragmentShaderPath) : window(gameDriver->Engine().Window()), drawables(), updatables() {
  driver = gameDriver;
  shaders = OpenGLUtility::LoadShaders(vertexShaderPath.c_str(), fragmentShaderPath.c_str());
  transformID = glGetUniformLocation(shaders, "transform");

  OpenGLRectangle* tile =  new ZeldaFindingScreen();
  tile->MoveTo(glm::vec3(400, 200 , 0.005));
  std::shared_ptr<Bindable> cardObject(tile);
  BindObject(cardObject);

  for (int level = 0; level < 8; ++level) {
    OpenGLRectangle* marker =  new TriforceMarker(level);
    marker->MoveTo(glm::vec3(45 + 65*level, 400-20-5,0.000));
    std::shared_ptr<Bindable> marObject(marker);
    BindObject(marObject);
  }
  for (int level = 0; level < 8; ++level) {
    OpenGLRectangle* marker =  new HeartMarker(level);
    marker->MoveTo(glm::vec3(45 + 65*level, 400-20-5-40,0.000));
    std::shared_ptr<Bindable> marObject(marker);
    BindObject(marObject);
  }
  for (int level = 0; level < 8; ++level) {
    if (level != 1) {
      OpenGLRectangle* marker =  new ItemMarker(level, true);
      marker->MoveTo(glm::vec3(45 + 65*level, 400-20-5-40-40,0.000));
      std::shared_ptr<Bindable> marObject(marker);
      BindObject(marObject);
    }
    if (level == 7) {
      OpenGLRectangle* marker =  new ItemMarker(level, false);
      marker->MoveTo(glm::vec3(45 + 65*(level+1), 400-20-5-40-40,0.000));
      std::shared_ptr<Bindable> marObject(marker);
      BindObject(marObject);
    }
  }
  {
    OpenGLRectangle* marker =  new ItemMarker(8, false);
    marker->MoveTo(glm::vec3(45 + 65*(8+1), 400-20-5-40,0.000));
    std::shared_ptr<Bindable> marObject(marker);
    BindObject(marObject);
  }  
  {
    OpenGLRectangle* marker =  new ItemMarker(8, true);
    marker->MoveTo(glm::vec3(45 + 65*(8+1), 400-20-5-40-40,0.000));
    std::shared_ptr<Bindable> marObject(marker);
    BindObject(marObject);
  }
  for (int x = 0; x < 2; ++x) {
    for (int y = 0; y < 2; ++y) {    
      OpenGLRectangle* marker =  new BonusCaveMarker(x+2*y);
      marker->MoveTo(glm::vec3(45 + 65*(x+10), 400-20-5-40*(y+1),0.000));
      std::shared_ptr<Bindable> marObject(marker);
      BindObject(marObject);
    }
  }

  {
    OpenGLRectangle* tile =  new OverworldMap();
    tile->MoveTo(glm::vec3(400, 262.5/2,0));
    std::shared_ptr<Bindable> cardObject(tile);    
    BindObject(cardObject);
  }
  
  for (int row = 0; row < 8; ++row) {
    for (int col = 0; col < 16; ++col) {
      double px = 50;
      double py = 50.0*336.0/512.0;
      OpenGLRectangle* marker =  new OverworldMarker(col, 7-row);
      marker->MoveTo(glm::vec3(px*col + px/2, py*row+py/2,0.001));                 
      std::shared_ptr<Bindable> markerObject(marker);
      BindObject(markerObject);
    }                                                                        
  }
  OpenGLRectangle* selector =  new OverworldSelector();
  std::shared_ptr<Bindable> selObject(selector);
  BindObject(selObject);  
}

void OverworldState::BindObject(std::shared_ptr<Bindable> object) {
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

void OverworldState::UnbindObject(std::shared_ptr<Bindable> object) {    
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

void OverworldState::EnterFocus() {

}

void OverworldState::LeaveFocus() {

}

void OverworldState::Update() {
    for (std::shared_ptr<Updatable> object : updatables) {
        object->Update();
    }
    if (ZeldaInformationHandler::GetIsInDungeon()) {
      auto os = driver->Engine().PopState();
      auto ds = driver->Engine().PopState();
      driver->Engine().PushState(os);
      driver->Engine().PushState(ds);
    }
}

void OverworldState::Draw(double time) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(shaders);

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

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
