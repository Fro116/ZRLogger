#include "OptionButton.h"

#include "ZeldaInformationHandler.h"

OptionButton::OptionButton(float width, float height, std::string normalTexture, std::string mouseTexture, bool quest, bool random) :
  OpenGLButton(width, height, normalTexture, mouseTexture),
  width(width),
  height(height),
  quest(quest),
  random(random)
{

}

bool OptionButton::Mouseover() {
  double xpos, ypos;
  int xwin, ywin;
  glfwGetCursorPos(window, &xpos, &ypos);
  glfwGetWindowSize(window, &xwin, &ywin);
  glm::vec3 pos = GetPosition();
  glm::vec3 scale = GetScale();  
  xpos = (xpos/xwin) * 800;
  ypos = (ywin - ypos)/ywin * 400;
  return (xpos < pos[0] + width * scale[0]/2 && xpos > pos[0] - width * scale[0]/2 && ypos < pos[1] + height * scale[1]/2 && ypos > pos[1] - height * scale[1]/2);
}

void OptionButton::Click() {
  ZeldaInformationHandler::SetOptions(quest, random);
}
