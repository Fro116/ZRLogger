//
//  HeartsEngine.cpp
//  Hearts
//
//  Created by Kundan Chintamaneni on 2/21/15.
//  Copyright (c) 2015 Kundan Chintamaneni. All rights reserved.
//

#include "HeartsEngine.h"

#include "OpenGLUtility.h"

HeartsEngine::HeartsEngine() : states(), isRunning(true) {
    GLFWwindow* window = OpenGLUtility::InitializeWindow("Hearts", 800, 600);
    this->window = window;
}


void HeartsEngine::PushState(std::shared_ptr<GameState>  state) {
    if (!states.empty()) {
        states.top()->HideState();
    }
    state->EnterStack();
    states.push(state);
}

std::shared_ptr<GameState>  HeartsEngine::PopState() {
    std::shared_ptr<GameState> state = states.top();
    state->LeaveStack();
    states.pop();
    
    states.top()->RevealState();
    
    return state;
}

void HeartsEngine::HandleEvents() {
    states.top()->HandleEvents();
}

void HeartsEngine::Update() {
    isRunning = glfwWindowShouldClose(window) == 0;
    states.top()->Update();
}

void HeartsEngine::Draw(double time) {
    states.top()->Draw(time);
}

bool HeartsEngine::IsRunning() {
    return isRunning;
}

void HeartsEngine::Quit() {
    isRunning = false;
}

GLFWwindow* HeartsEngine::Window() {
    return window;
}
