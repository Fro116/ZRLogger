//
//  HeartsDrawable.cpp
//  Hearts
//
//  Created by Kundan Chintamaneni on 3/13/15.
//  Copyright (c) 2015 Kundan Chintamaneni. All rights reserved.
//

#include "Drawable.h"

glm::mat4 Drawable::CameraTransform() const {
    return transform;
}

void Drawable::SetCameraTransform(const glm::mat4 &transform) {
    this->transform = transform;
}

GLuint Drawable::TransformID() const {
    return transformID;
}

void Drawable::SetTransformID(GLuint transformID) {
    this->transformID = transformID;
}
