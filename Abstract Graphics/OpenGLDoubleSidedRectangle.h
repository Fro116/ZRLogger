//
//  OpenGlPlayingCard.h
//  Hearts
//
//  Created by Kundan Chintamaneni on 2/22/15.
//  Copyright (c) 2015 Kundan Chintamaneni. All rights reserved.
//

#ifndef __Hearts__OpenGlDoubleSidedRectangle__
#define __Hearts__OpenGlDoubleSidedRectangle__

#include <iostream>

#include "Updatable.h"
#include "OpenGLUtility.h"
#include "Bindable.h"
#include "Movable.h"
#include "Drawable.h"

class OpenGLDoubleSidedRectangle : public Bindable, public Updatable, public Drawable, public Movable {
public:
  OpenGLDoubleSidedRectangle(float width, float height, std::string frontTexturePath, std::string backTexturePath);
  void Bind(const GameState* state) override;
  void Unbind(const GameState* state) override;
  void Update() override;
  void Draw(double time) override;
 private:
  GLuint model;
  GLuint frontTexture;
  GLuint backTexture;
};

#endif /* defined(__Hearts__OpenGlPlayingCard__) */
