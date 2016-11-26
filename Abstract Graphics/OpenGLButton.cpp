#include "OpenGLButton.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

OpenGLButton::OpenGLButton(float width, float height, std::string normalTexture, std::string mouseTexture) {
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
    normalTex = OpenGLUtility::Load2DTexture(normalTexture.c_str(), GL_RGBA);
    mouseTex = OpenGLUtility::Load2DTexture(mouseTexture.c_str(), GL_RGBA);    
}

OpenGLButton::OpenGLButton(float width, float height, GLuint normalTexture, GLuint mouseTexture) {
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
    normalTex = normalTexture;
    mouseTex = mouseTexture;
}

void OpenGLButton::Bind(GameState *state, GLFWwindow* win) {
  window = win;
}

void OpenGLButton::Unbind() {
    
}

void OpenGLButton::Update() {

}

void OpenGLButton::ClickHandler(int button, int action, int mods) {
  if (Mouseover() && button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
    Click();
  }
}

void OpenGLButton::Draw(double time) {
  GLuint texture = normalTex;
  if (Mouseover()) {
    texture = mouseTex;
  }
  
  glm::mat4 PVM = CameraTransform() * ModelTranform();
    
  glBindVertexArray(model);
  glUniformMatrix4fv(TransformID(), 1, GL_FALSE, &PVM[0][0]);

  glBindTexture(GL_TEXTURE_2D, texture);
  glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_SHORT, 0);
    
  glBindTexture(GL_TEXTURE_2D, 0);
  glBindVertexArray(0);
}
