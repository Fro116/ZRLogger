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
    virtual void EnterFocus() = 0;
    virtual void LeaveFocus() = 0;
    
    virtual void Update() = 0;
    virtual void Draw(double time) = 0;
private:
};

#endif /* defined(__Hearts__GameState__) */
