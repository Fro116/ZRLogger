//
//  OpenGlPlayingCard.h
//  Hearts
//
//  Created by Kundan Chintamaneni on 2/22/15.
//  Copyright (c) 2015 Kundan Chintamaneni. All rights reserved.
//

#ifndef __Hearts__OpenGlPlayingCard__
#define __Hearts__OpenGlPlayingCard__

#include <iostream>

#include "PlayingCard.h"
#include "Updatable.h"
#include "OpenGLUtility.h"
#include "Bindable.h"
#include "Movable.h"
#include "Drawable.h"

//TODO translate, rotate, scale
//TODO texture
//Todo backface

class OpenGlPlayingCard : public Bindable, public Updatable, public Drawable, public Movable {
public:
    OpenGlPlayingCard(const PlayingCard card);
    void Bind(const GameState* state) override;
    void Unbind(const GameState* state) override;
    void Update() override;
    void Draw(double time) override;
private:
    std::string TextureName(const PlayingCard card) const;
    
    const PlayingCard card;
    GLuint model;
    GLuint frontTexture;
    GLuint backTexture;
};

#endif /* defined(__Hearts__OpenGlPlayingCard__) */
