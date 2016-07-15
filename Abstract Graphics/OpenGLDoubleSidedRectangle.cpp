//
//  OpenGlPlayingCard.cpp
//  Hearts
//
//  Created by Kundan Chintamaneni on 2/22/15.
//  Copyright (c) 2015 Kundan Chintamaneni. All rights reserved.
//

#include "OpenGLDoubleSidedRectangle.h"

#include <iostream>

OpenGLDoubleSidedRectangle::OpenGLDoubleSidedRectangle(float width, float height, std::string frontTexturePath, std::string backTexturePath) {
    GLuint vertexArray;
    glGenVertexArrays(1, &vertexArray);
    glBindVertexArray(vertexArray);
    
    GLfloat vertexBufferData[] = {
        -width/2, -height/2,
        -width/2, height/2,
        width/2, height/2,
        width/2, -height/2,
        -width/2, -height/2,
        -width/2, height/2,
        width/2, height/2,
        width/2, -height/2
    }; //BL, BR, TR, TL
    GLfloat textureBufferData[] = {
        0, 0,
        0, 1,
        1, 1,
        1, 0,
        0, 1,
        0, 0,
        1, 0,
        1, 1
    };
    GLushort elementBufferData[] = {0, 1, 3, 2, 7, 6, 4, 5};
    
    model = OpenGLUtility::CreateVertexArrayObject(vertexBufferData, sizeof(vertexBufferData), 2, textureBufferData, sizeof(textureBufferData), 2, elementBufferData, sizeof(elementBufferData));
    
    frontTexture = OpenGLUtility::Load2DTexture(frontTexturePath.c_str(), GL_RGBA);
    backTexture = OpenGLUtility::Load2DTexture(backTexturePath.c_str(), GL_RGBA);
}

void OpenGLDoubleSidedRectangle::Bind(const GameState *state) {
    
}

void OpenGLDoubleSidedRectangle::Unbind(const GameState *state) {
    
}

void OpenGLDoubleSidedRectangle::Update() {

}

void OpenGLDoubleSidedRectangle::Draw(double time) {
    glm::mat4 PVM = CameraTransform() * ModelTranform();
    
    glBindVertexArray(model);
    glUniformMatrix4fv(TransformID(), 1, GL_FALSE, &PVM[0][0]);
    
    glBindTexture(GL_TEXTURE_2D, backTexture);
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_SHORT, 0);
    
    glBindTexture(GL_TEXTURE_2D, frontTexture);
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_SHORT, reinterpret_cast<GLvoid*>(4 * sizeof(GLushort)));
    
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
}

