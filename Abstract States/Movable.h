//
//  HeartsMovable.h
//  Hearts
//
//  Created by Kundan Chintamaneni on 3/13/15.
//  Copyright (c) 2015 Kundan Chintamaneni. All rights reserved.
//

#ifndef __Hearts__HeartsMovable__
#define __Hearts__HeartsMovable__

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

class Movable {
public:
    Movable();
    
    glm::mat4 ModelTranform();
    glm::vec3 GetPosition();
    glm::vec3 GetScale();
    glm::quat GetRotation();    
    void MoveTo(glm::vec3 position);
    void ScaleTo(glm::vec3 scale);
    void RotateTo(glm::quat rotation);
    void MoveBy(glm::vec3 position);
    void ScaleBy(glm::vec3 scale);
    void RotateBy(glm::quat rotation);
private:
    glm::vec3 position;
    glm::vec3 scale;
    glm::quat rotation;
};

#endif /* defined(__Hearts__HeartsMovable__) */
