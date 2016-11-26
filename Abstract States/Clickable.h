#ifndef Clickable_1480122353021522
#define Clickable_1480122353021522

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Clickable {
 public:
  virtual void ClickHandler(int button, int action, int mods) = 0;
 private:
};

#endif
