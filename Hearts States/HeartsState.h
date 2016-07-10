//
//  HeartsState.h
//  Hearts
//
//  Created by Kundan Chintamaneni on 2/21/15.
//  Copyright (c) 2015 Kundan Chintamaneni. All rights reserved.
//

#ifndef __Hearts__HeartsState__
#define __Hearts__HeartsState__

#include <memory>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "GameState.h"
#include "Drawable.h"
#include "Updatable.h"
#include "Bindable.h"

class HeartsState : public GameState {
public:
    static std::shared_ptr<HeartsState> CreateInstance(GLFWwindow* window);
    ~HeartsState();
    
    void BindObject(std::shared_ptr<Bindable> object);
    void UnbindObject(std::shared_ptr<Bindable> object);
    
    void EnterFocus() override;
    void LeaveFocus() override;
    
    void Update() override;
    void Draw(double time) override;
private:
    HeartsState(GLFWwindow* window);
    GLFWwindow* window;
    GLuint shaders;
    GLuint transformID;
    std::vector<std::shared_ptr<Bindable>> bindables;
    std::vector<std::shared_ptr<Drawable>> drawables;
    std::vector<std::shared_ptr<Updatable>> updatables;
};

#endif /* defined(__Hearts__HeartsState__) */
