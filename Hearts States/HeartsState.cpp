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

#include "OpenGLUtility.h"

std::shared_ptr<HeartsState> HeartsState::CreateInstance(GLFWwindow* window) {
    return std::shared_ptr<HeartsState>(new HeartsState(window));
}

HeartsState::HeartsState(GLFWwindow* window) : window(window), drawables(), updatables() {
//    shaders = OpenGLUtility::LoadShaders("Hearts Graphics/Shaders/ColorShader.vertexshader", "Hearts Graphics/Shaders/ColorShader.fragmentshader");
    shaders = OpenGLUtility::LoadShaders("Hearts Graphics/Shaders/TextureShader.vertexshader", "Hearts Graphics/Shaders/TextureShader.fragmentshader");
     transformID = glGetUniformLocation(shaders, "transform");
}

void HeartsState::BindObject(std::shared_ptr<Bindable> object) {
    bindables.push_back(object);
    
    std::shared_ptr<Updatable> updatable = std::dynamic_pointer_cast<Updatable>(object);
    if (updatable) {
        updatables.push_back(updatable);
    }
    
    std::shared_ptr<HeartsDrawable> drawable = std::dynamic_pointer_cast<HeartsDrawable>(object);
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
    
    std::shared_ptr<HeartsDrawable> drawable = std::dynamic_pointer_cast<HeartsDrawable>(object);
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

void HeartsState::EnterStack() {

}

void HeartsState::LeaveStack() {

}

void HeartsState::HideState() {
    
}

void HeartsState::RevealState() {
    
}

void HeartsState::HandleEvents() {
    
}

void HeartsState::Update() {
    for (std::shared_ptr<Updatable> object : updatables) {
        object->Update();
    }
}

void HeartsState::Draw(double time) {
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glUseProgram(shaders);
    
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    float aspectRatio = static_cast<float>(width)/static_cast<float>(height);
    
    glm::mat4 Projection = glm::perspective(glm::pi<float>()/2, aspectRatio, 10.0f, 1000.0f);
    glm::mat4 View       = glm::lookAt(glm::vec3(400,300,300), glm::vec3(400,300,0), glm::vec3(0,1,0));
    glm::mat4 PV       = Projection * View;
    
//    glm::mat4 Projection = glm::perspective(glm::pi<float>()/2, 1.0f, 300.0f, 1000.0f);
//    glm::mat4 View       = glm::lookAt(glm::vec3(400,300,-300), glm::vec3(400,300,0), glm::vec3(0,1,0));
//    glm::mat4 PV       = Projection * View;

    
//    std::cout << Projection[0][0] << "\t" << Projection[0][1] << "\t" << Projection[0][2] << "\t" << Projection[0][3] << "\n";
//    std::cout << Projection[1][0] << "\t" << Projection[1][1] << "\t" << Projection[1][2] << "\t" << Projection[1][3] << "\n";
//    std::cout << Projection[2][0] << "\t" << Projection[2][1] << "\t" << Projection[2][2] << "\t" << Projection[2][3] << "\n";
//    std::cout << Projection[3][0] << "\t" << Projection[3][1] << "\t" << Projection[3][2] << "\t" << Projection[3][3] << "\n";

//    glm::vec4 vector(0,0,0,1);
//    std::cout << (View * vector).x << "\t";
//    std::cout << (View * vector).y << "\t";
//    std::cout << (View * vector).z << "\t";
//    std::cout << (View * vector).w << "\t";
//    std::cout << "\n";
//    std::cout << (PV * vector).x << "\t";
//    std::cout << (PV * vector).y << "\t";
//    std::cout << (PV * vector).z << "\t";
//    std::cout << (PV * vector).w << "\t";
//    std::cout << "\n";
//    std::cout << "\n";

    
    for (auto object : drawables) {
        object->SetCameraTransform(PV);
        object->SetTransformID(transformID);
        object->Draw(time);
    }
    
    glfwSwapBuffers(window);
    
    glUseProgram(0);
    
    glfwPollEvents();
}
