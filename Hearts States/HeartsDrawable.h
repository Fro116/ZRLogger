//
//  HeartsDrawable.h
//  Hearts
//
//  Created by Kundan Chintamaneni on 3/13/15.
//  Copyright (c) 2015 Kundan Chintamaneni. All rights reserved.
//

#ifndef __Hearts__HeartsDrawable__
#define __Hearts__HeartsDrawable__

#include <glm/glm.hpp>
#include <GL/glew.h>
#include "Drawable.h"

class HeartsDrawable : public Drawable {
public:
    glm::mat4 CameraTransform() const;
    void SetCameraTransform(const glm::mat4& transform);
    GLuint TransformID() const;
    void SetTransformID(GLuint transformID);
private:
    glm::mat4 transform;
    GLuint transformID;
};

#endif /* defined(__Hearts__HeartsDrawable__) */
