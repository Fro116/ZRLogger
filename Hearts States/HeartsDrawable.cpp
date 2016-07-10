//
//  HeartsDrawable.cpp
//  Hearts
//
//  Created by Kundan Chintamaneni on 3/13/15.
//  Copyright (c) 2015 Kundan Chintamaneni. All rights reserved.
//

#include "HeartsDrawable.h"

glm::mat4 HeartsDrawable::CameraTransform() const {
    return transform;
}

void HeartsDrawable::SetCameraTransform(const glm::mat4 &transform) {
    this->transform = transform;
}

GLuint HeartsDrawable::TransformID() const {
    return transformID;
}

void HeartsDrawable::SetTransformID(GLuint transformID) {
    this->transformID = transformID;
}
