//
//  HeartsEngine.h
//  Hearts
//
//  Created by Kundan Chintamaneni on 2/21/15.
//  Copyright (c) 2015 Kundan Chintamaneni. All rights reserved.
//

#ifndef __Hearts__HeartsEngine__
#define __Hearts__HeartsEngine__

#include "GameEngine.h"

#include <stack>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class HeartsEngine : public GameEngine {
public:
    HeartsEngine();
    
    void PushState(std::shared_ptr<GameState>  state) override;
    std::shared_ptr<GameState> PopState() override;
    
    void HandleEvents() override;
    void Update() override;
    void Draw(double time) override;
    
    bool IsRunning() override;
    void Quit() override;
    
    GLFWwindow* Window();
private:
    std::stack<std::shared_ptr<GameState>> states;
    bool isRunning;
    GLFWwindow* window;
};

#endif /* defined(__Hearts__HeartsEngine__) */
