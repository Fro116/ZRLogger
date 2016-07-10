//
//  HeartsMovable.cpp
//  Hearts
//
//  Created by Kundan Chintamaneni on 3/13/15.
//  Copyright (c) 2015 Kundan Chintamaneni. All rights reserved.
//

#include "HeartsMovable.h"

#include <glm/gtx/transform.hpp>

#include <iostream>

HeartsMovable::HeartsMovable() : position(0,0,0), scale(1,1,1), rotation(1,0,0,0) {
    
}

glm::mat4 HeartsMovable::ModelTranform() {
    return glm::translate(position) * glm::mat4_cast(glm::normalize(rotation)) * glm::scale(scale);
}
void HeartsMovable::MoveTo(glm::vec3 position) {
    this->position = position;
}

void HeartsMovable::ScaleTo(glm::vec3 scale) {
    this->scale = scale;
}

void HeartsMovable::RotateTo(glm::quat rotation) {
    this->rotation = rotation;
}

void HeartsMovable::MoveBy(glm::vec3 position) {
    this-> position += position;
}

void HeartsMovable::ScaleBy(glm::vec3 scale) {
    this->scale *= scale;
}

void HeartsMovable::RotateBy(glm::quat rotation) {
    this->rotation = rotation * this->rotation;
}