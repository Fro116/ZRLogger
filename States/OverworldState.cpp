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

#include "OpenGLDoubleSidedRectangle.h"
#include "OpenGLRectangle.h"

#include "OpenGLUtility.h"

std::shared_ptr<OverworldState> OverworldState::CreateInstance(GLFWwindow* window, std::string vertexShaderPath, std::string fragmentShaderPath) {
  return std::shared_ptr<OverworldState>(new OverworldState(window, vertexShaderPath, fragmentShaderPath));
}

OverworldState::OverworldState(GLFWwindow* window, std::string vertexShaderPath, std::string fragmentShaderPath) : window(window), drawables(), updatables() {
  shaders = OpenGLUtility::LoadShaders(vertexShaderPath.c_str(), fragmentShaderPath.c_str());
  transformID = glGetUniformLocation(shaders, "transform");

  std::string alphabet[] = {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P"};
  for (int row = 0; row < 7; ++row) {
    for (int col = 0; col < 16; ++col) {
      double px = 50;
      double py = 50.0*336.0/512.0;
      std::string num = std::to_string(8-row);
      std::string let = alphabet[col];
      OpenGLRectangle* tile =  new OpenGLRectangle(px, py, "Images/Overworld/" + num + let + ".png");
      tile->MoveTo(glm::vec3(px*col + px/2, py*row+py/2,0));                 
      std::shared_ptr<Bindable> cardObject(tile);                            
      BindObject(cardObject);                                                
    }                                                                        
  }  
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

OverworldState::~OverworldState() {
    for (auto object : bindables) {
        UnbindObject(object);
    }
}

void OverworldState::EnterFocus() {

}

void OverworldState::LeaveFocus() {

}

void OverworldState::Update() {
    for (std::shared_ptr<Updatable> object : updatables) {
        object->Update();
    }
}

void OverworldState::Draw(double time) {
    //glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(shaders);

    int x = 800;
    int y = 600;
    int z = x/2;
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    float aspectRatio = static_cast<float>(width)/static_cast<float>(height);
    float fov = 2*glm::atan(static_cast<float>(height)/(2*z));
    glm::mat4 Projection = glm::perspective(fov, aspectRatio, 10.0f, 1000.0f);
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
