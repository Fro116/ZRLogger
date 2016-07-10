//
//  Bindable.h
//  Hearts
//
//  Created by Kundan Chintamaneni on 3/13/15.
//  Copyright (c) 2015 Kundan Chintamaneni. All rights reserved.
//

#ifndef __Hearts__Bindable__
#define __Hearts__Bindable__

#include <vector>

#include "GameState.h"

class Bindable {
public:
    virtual void Bind(const GameState* state) = 0;
    virtual void Unbind(const GameState* state) = 0;
};

#endif /* defined(__Hearts__Bindable__) */
