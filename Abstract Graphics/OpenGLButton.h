#ifndef OpenGLButton_1480111158761834
#define OpenGLButton_1480111158761834

#include <iostream>

#include "Updatable.h"
#include "OpenGLUtility.h"
#include "Bindable.h"
#include "Movable.h"
#include "Drawable.h"
#include "Clickable.h"

class OpenGLButton : public Bindable, public Updatable, public Drawable, public Movable, public Clickable {
public:
  OpenGLButton(float width, float height, GLuint normalTexture, GLuint mouseTexture);  
  OpenGLButton(float width, float height, std::string normalTexture, std::string mouseTexture);
  void Bind(GameState* state, GLFWwindow* window) override;
  void Unbind() override;
  void Update() override;
  void Draw(double time) override;
  void ClickHandler(int button, int action, int mods) override;
  virtual bool Mouseover() = 0;
  virtual void Click() = 0;
 protected:
  GLFWwindow* window;
 private:
  GLuint model;
  GLuint normalTex;
  GLuint mouseTex;
};

#endif
