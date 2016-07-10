//
//  Drawable.h
//  Hearts
//
//  Created by Kundan Chintamaneni on 3/9/15.
//  Copyright (c) 2015 Kundan Chintamaneni. All rights reserved.
//

#ifndef Hearts_Drawable_h
#define Hearts_Drawable_h

#include <glm/glm.hpp>
#include <GL/glew.h>

class Drawable {
 public:
  virtual void Draw(double time) = 0;
  glm::mat4 CameraTransform() const;
  void SetCameraTransform(const glm::mat4& transform);
  GLuint TransformID() const;
  void SetTransformID(GLuint transformID);
 private:
  glm::mat4 transform;
  GLuint transformID;
};

#endif
