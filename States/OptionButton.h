#ifndef OptionButton_1480114622066347
#define OptionButton_1480114622066347

#include "OpenGLButton.h"

class OptionButton : public OpenGLButton {
 public:
  OptionButton(float width, float height, std::string normalTexture, std::string mouseTexture, bool firstQuest, bool random);
  bool Mouseover() override;
  void Click() override;
 private:
  bool quest;
  bool random;
  double width;
  double height;
};

#endif
