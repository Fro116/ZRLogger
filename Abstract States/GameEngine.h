//
//  GameEngine.h
//  Hearts
//
//  Created by Kundan Chintamaneni on 2/21/15.
//  Copyright (c) 2015 Kundan Chintamaneni. All rights reserved.
//

#ifndef __Hearts__GameEngine__
#define __Hearts__GameEngine__

#include "GameState.h"

#include <memory>

class GameEngine {
public:
    virtual void PushState(std::shared_ptr<GameState>  state) = 0;
    virtual std::shared_ptr<GameState>  PopState() = 0;
    
    virtual void HandleEvents() = 0;
    virtual void Update() = 0;
    virtual void Draw(double time) = 0;
    
    virtual bool IsRunning() = 0;
    virtual void Quit() = 0;
private:
};

#endif /* defined(__Hearts__GameEngine__) */
