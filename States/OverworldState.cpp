//
//  HeartsState.cpp
//  Hearts
//
//  Created by Kundan Chintamaneni on 2/21/15.
//  Copyright (c) 2015 Kundan Chintamaneni. All rights reserved.
//

#include "HeartsState.h"

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include "OpenGLDoubleSidedRectangle.h"
#include "OpenGLRectangle.h"

#include "OpenGLUtility.h"

std::shared_ptr<HeartsState> HeartsState::CreateInstance(GLFWwindow* window, std::string vertexShaderPath, std::string fragmentShaderPath) {
  return std::shared_ptr<HeartsState>(new HeartsState(window, vertexShaderPath, fragmentShaderPath));
}

HeartsState::HeartsState(GLFWwindow* window, std::string vertexShaderPath, std::string fragmentShaderPath) : window(window), drawables(), updatables() {
  shaders = OpenGLUtility::LoadShaders(vertexShaderPath.c_str(), fragmentShaderPath.c_str());
  transformID = glGetUniformLocation(shaders, "transform");

  OpenGLRectangle* cardImage =  new OpenGLRectangle(0.726*2,0.726*2, "Images/Overworld/1A.png");
  cardImage->MoveTo(glm::vec3(400,300,0));
  cardImage->ScaleBy(glm::vec3(100,100,0));
  std::shared_ptr<Bindable> cardObject(cardImage);
  BindObject(cardObject);
}

void HeartsState::BindObject(std::shared_ptr<Bindable> object) {
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

void HeartsState::UnbindObject(std::shared_ptr<Bindable> object) {    
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

HeartsState::~HeartsState() {
    for (auto object : bindables) {
        UnbindObject(object);
    }
}

void HeartsState::EnterFocus() {

}

void HeartsState::LeaveFocus() {

}

void HeartsState::Update() {
    for (std::shared_ptr<Updatable> object : updatables) {
        object->Update();
    }
}

void HeartsState::Draw(double time) {
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
