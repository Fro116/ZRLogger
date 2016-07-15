//
//  OpenGlPlayingCard.cpp
//  Hearts
//
//  Created by Kundan Chintamaneni on 2/22/15.
//  Copyright (c) 2015 Kundan Chintamaneni. All rights reserved.
//

#include "OpenGLRectangle.h"

#include <iostream>

OpenGLRectangle::OpenGLRectangle(float width, float height, std::string texturePath) {
    GLfloat vertexBufferData[] = {
        -width/2, -height/2,
        -width/2, height/2,
        width/2, height/2,
        width/2, -height/2
    }; //BL, BR, TR, TL
    GLfloat textureBufferData[] = {
        0, 1,
        0, 0,
        1, 0,
        1, 1
    };
    GLushort elementBufferData[] = {3, 2, 0, 1};
    
    model = OpenGLUtility::CreateVertexArrayObject(vertexBufferData, sizeof(vertexBufferData), 2, textureBufferData, sizeof(textureBufferData), 2, elementBufferData, sizeof(elementBufferData));
    std::cout << "MODEL IS: " << model << std::endl;
    std::cout << "VB IS: " << vertexBufferData[0] << " " << vertexBufferData[1] << std::endl;
    texture = OpenGLUtility::Load2DTexture(texturePath.c_str(), GL_RGBA);
}

void OpenGLRectangle::Bind(const GameState *state) {
    
}

void OpenGLRectangle::Unbind(const GameState *state) {
    
}

void OpenGLRectangle::Update() {

}

void OpenGLRectangle::SetTexture(std::string texturePath) {
    texture = OpenGLUtility::Load2DTexture(texturePath.c_str(), GL_RGBA);
}

void OpenGLRectangle::Draw(double time) {
    glm::mat4 PVM = CameraTransform() * ModelTranform();
    
    glBindVertexArray(model);
    glUniformMatrix4fv(TransformID(), 1, GL_FALSE, &PVM[0][0]);

    glBindTexture(GL_TEXTURE_2D, texture);
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_SHORT, 0);
    
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
}

