//
//  OpenGlPlayingCard.cpp
//  Hearts
//
//  Created by Kundan Chintamaneni on 2/22/15.
//  Copyright (c) 2015 Kundan Chintamaneni. All rights reserved.
//

#include "OpenGlPlayingCard.h"

#include <iostream>

#include "Random.h"

OpenGlPlayingCard::OpenGlPlayingCard(const PlayingCard card) : card(card) {
    GLuint vertexArray;
    glGenVertexArrays(1, &vertexArray);
    glBindVertexArray(vertexArray);
    
    static const GLfloat vertexBufferData[] = {
        -0.5f, -0.726f,
        -0.5f, 0.726f,
        0.5f, 0.726f,
        0.5f, -0.726f,
        -0.5f, -0.726f,
        -0.5f, 0.726f,
        0.5f, 0.726f,
        0.5f, -0.726f
    }; //BL, BR, TR, TL
    static const GLfloat textureBufferData[] = {
        0, 0,
        0, 1,
        1, 1,
        1, 0,
        0, 1,
        0, 0,
        1, 0,
        1, 1
    };
    static const GLushort elementBufferData[] = {0, 1, 3, 2, 7, 6, 4, 5};
    
    model = OpenGLUtility::CreateVertexArrayObject(vertexBufferData, sizeof(vertexBufferData), 2, textureBufferData, sizeof(textureBufferData), 2, elementBufferData, sizeof(elementBufferData));
    
    frontTexture = OpenGLUtility::Load2DTexture(TextureName(card).c_str(), GL_RGBA);
    backTexture = OpenGLUtility::Load2DTexture("Hearts Graphics/CardBackBlue.png", GL_RGBA);
}

void OpenGlPlayingCard::Bind(const GameState *state) {
    
}

void OpenGlPlayingCard::Unbind(const GameState *state) {
    
}

void OpenGlPlayingCard::Update() {
    RotateBy(glm::angleAxis(0.01f, glm::vec3(1,1,0)));
}

void OpenGlPlayingCard::Draw(double time) {
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

std::string OpenGlPlayingCard::TextureName(const PlayingCard card) const {
    std::string textureName = "Hearts Graphics/Playing Cards/";
    switch (card.Rank()) {
        case PlayingCard::Rank::TWO:
            textureName+="2";
            break;
        case PlayingCard::Rank::THREE:
            textureName+="3";
            break;
        case PlayingCard::Rank::FOUR:
            textureName+="4";
            break;
        case PlayingCard::Rank::FIVE:
            textureName+="5";
            break;
        case PlayingCard::Rank::SIX:
            textureName+="6";
            break;
        case PlayingCard::Rank::SEVEN:
            textureName+="7";
            break;
        case PlayingCard::Rank::EIGHT:
            textureName+="8";
            break;
        case PlayingCard::Rank::NINE:
            textureName+="9";
            break;
        case PlayingCard::Rank::TEN:
            textureName+="10";
            break;
        case PlayingCard::Rank::JACK:
            textureName+="J";
            break;
        case PlayingCard::Rank::QUEEN:
            textureName+="Q";
            break;
        case PlayingCard::Rank::KING:
            textureName+="K";
            break;
        case PlayingCard::Rank::ACE:
            textureName+="A";
            break;
    }
    switch (card.Suit()) {
        case PlayingCard::Suit::CLUBS:
            textureName+="C";
            break;
        case PlayingCard::Suit::DIAMONDS:
            textureName+="D";
            break;
        case PlayingCard::Suit::HEARTS:
            textureName+="H";
            break;
        case PlayingCard::Suit::SPADES:
            textureName+="S";
            break;
    }
    textureName += ".png";
    return textureName;
}

