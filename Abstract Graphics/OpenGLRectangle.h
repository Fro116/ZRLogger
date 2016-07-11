//
//  OpenGlPlayingCard.h
//  Hearts
//
//  Created by Kundan Chintamaneni on 2/22/15.
//  Copyright (c) 2015 Kundan Chintamaneni. All rights reserved.
//

#ifndef __Hearts__OpenGlRectangle__
#define __Hearts__OpenGlRectangle__

#include <iostream>

#include "Updatable.h"
#include "OpenGLUtility.h"
#include "Bindable.h"
#include "Movable.h"
#include "Drawable.h"

class OpenGLRectangle : public Bindable, public Updatable, public Drawable, public Movable {
public:
  OpenGLRectangle(float width, float height, std::string texturePath);
  void Bind(const GameState* state) override;
  void Unbind(const GameState* state) override;
  void Update() override;
  void Draw(double time) override;
 private:
  GLuint model;
  GLuint texture;
};

#endif /* defined(__Hearts__OpenGlPlayingCard__) */
