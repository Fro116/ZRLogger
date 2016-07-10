//
//  GameState.h
//  Hearts
//
//  Created by Kundan Chintamaneni on 2/21/15.
//  Copyright (c) 2015 Kundan Chintamaneni. All rights reserved.
//

#ifndef __Hearts__GameState__
#define __Hearts__GameState__

class GameState {
public:
    virtual void EnterStack() = 0;
    virtual void LeaveStack() = 0;
    
    virtual void HideState() = 0;
    virtual void RevealState() = 0;
    
    virtual void HandleEvents() = 0;
    virtual void Update() = 0;
    virtual void Draw(double time) = 0;
private:
};

#endif /* defined(__Hearts__GameState__) */
