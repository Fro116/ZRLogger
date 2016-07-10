//
//  HeartsMovable.cpp
//  Hearts
//
//  Created by Kundan Chintamaneni on 3/13/15.
//  Copyright (c) 2015 Kundan Chintamaneni. All rights reserved.
//

#include "Movable.h"

#include <glm/gtx/transform.hpp>

#include <iostream>

Movable::Movable() : position(0,0,0), scale(1,1,1), rotation(1,0,0,0) {
    
}

glm::mat4 Movable::ModelTranform() {
    return glm::translate(position) * glm::mat4_cast(glm::normalize(rotation)) * glm::scale(scale);
}
void Movable::MoveTo(glm::vec3 position) {
    this->position = position;
}

void Movable::ScaleTo(glm::vec3 scale) {
    this->scale = scale;
}

void Movable::RotateTo(glm::quat rotation) {
    this->rotation = rotation;
}

void Movable::MoveBy(glm::vec3 position) {
    this-> position += position;
}

void Movable::ScaleBy(glm::vec3 scale) {
    this->scale *= scale;
}

void Movable::RotateBy(glm::quat rotation) {
    this->rotation = rotation * this->rotation;
}
