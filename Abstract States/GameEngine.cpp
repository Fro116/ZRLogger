//
//  HeartsEngine.cpp
//  Hearts
//
//  Created by Kundan Chintamaneni on 2/21/15.
//  Copyright (c) 2015 Kundan Chintamaneni. All rights reserved.
//

#include "GameEngine.h"

#include "OpenGLUtility.h"
GameEngine::GameEngine(std::string title, int screenWidth, int screenHeight) : states(), isRunning(true) {
    GLFWwindow* window = OpenGLUtility::InitializeWindow(title.c_str(), screenWidth, screenHeight);
    this->window = window;
}


void GameEngine::PushState(std::shared_ptr<GameState>  state) {
    if (!states.empty()) {
        states.top()->LeaveFocus();
    }
    state->EnterFocus();
    states.push(state);
}

std::shared_ptr<GameState>  GameEngine::PopState() {
    std::shared_ptr<GameState> state = states.top();
    state->LeaveFocus();
    states.pop();
    states.top()->EnterFocus();
    
    return state;
}

void GameEngine::Update() {
    isRunning = glfwWindowShouldClose(window) == 0;
    if (!states.empty()) 
      states.top()->Update();
}

void GameEngine::Draw(double time) {
    if (!states.empty()) 
      states.top()->Draw(time);
}

bool GameEngine::IsRunning() {
    return isRunning;
}

void GameEngine::Quit() {
    isRunning = false;
}

GLFWwindow* GameEngine::Window() {
    return window;
}
