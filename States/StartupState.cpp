#include "StartupState.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include "OpenGLRectangle.h"
#include "OptionButton.h"
#include "ZeldaInformationHandler.h"

StartupState::StartupState(std::shared_ptr<GameDriver> gameDriver, std::string vertexShaderPath, std::string fragmentShaderPath) : window(gameDriver->Engine().Window()), drawables(), updatables(), clickables() {
  driver = gameDriver;
  shaders = OpenGLUtility::LoadShaders(vertexShaderPath.c_str(), fragmentShaderPath.c_str());
  transformID = glGetUniformLocation(shaders, "transform");
  {
    OpenGLRectangle* tile =  new OpenGLRectangle(800, 90, "Images/StartupScreen/Title.png");
    tile->MoveTo(glm::vec3(400, 340 , 0));
    std::shared_ptr<Bindable> cardObject(tile);
    BindObject(cardObject);
  }      
  {
    OptionButton* tile =  new OptionButton(370, 115, "Images/StartupScreen/FirstNormalNormal.png", "Images/StartupScreen/FirstNormalMouse.png", true, false);
    tile->MoveTo(glm::vec3(200, 215 , 0));
    std::shared_ptr<Bindable> cardObject(tile);
    BindObject(cardObject);
  }
  {
    OptionButton* tile =  new OptionButton(370, 115, "Images/StartupScreen/FirstRandomNormal.png", "Images/StartupScreen/FirstRandomMouse.png", true, false);
    tile->MoveTo(glm::vec3(200, 90 , 0));
    std::shared_ptr<Bindable> cardObject(tile);
    BindObject(cardObject);
  }
  {
    OptionButton* tile =  new OptionButton(370, 115, "Images/StartupScreen/SecondNormalNormal.png", "Images/StartupScreen/SecondNormalMouse.png", false, false);
    tile->MoveTo(glm::vec3(600, 215 , 0));
    std::shared_ptr<Bindable> cardObject(tile);
    BindObject(cardObject);
  }
  {
    OptionButton* tile =  new OptionButton(370, 115, "Images/StartupScreen/SecondRandomNormal.png", "Images/StartupScreen/SecondRandomMouse.png", false, true);
    tile->MoveTo(glm::vec3(600, 90 , 0));
    std::shared_ptr<Bindable> cardObject(tile);
    BindObject(cardObject);
  }    

}

void StartupState::BindObject(std::shared_ptr<Bindable> object) {
    bindables.push_back(object);
    std::shared_ptr<Updatable> updatable = std::dynamic_pointer_cast<Updatable>(object);
    if (updatable) {
        updatables.push_back(updatable);
    }
    std::shared_ptr<Drawable> drawable = std::dynamic_pointer_cast<Drawable>(object);
    if (drawable) {
        drawables.push_back(drawable);
    }
    std::shared_ptr<Clickable> clickable = std::dynamic_pointer_cast<Clickable>(object);
    if (clickable) {
        clickables.push_back(clickable);
    }    
    object->Bind(this, window);
}

void StartupState::UnbindObject(std::shared_ptr<Bindable> object) {    
    std::shared_ptr<Updatable> updatable = std::dynamic_pointer_cast<Updatable>(object);
    if (updatable) {
        auto it = std::find(updatables.begin(), updatables.end(), updatable);
        if (it != updatables.end()) {
            updatables.erase(it);
        }
    }    
    std::shared_ptr<Drawable> drawable = std::dynamic_pointer_cast<Drawable>(object);
    if (drawable) {
        auto it = std::find(drawables.begin(), drawables.end(), drawable);
        if (it != drawables.end()) {
            drawables.erase(it);
        }
    }
    std::shared_ptr<Clickable> clickable = std::dynamic_pointer_cast<Clickable>(object);
    if (clickable) {
        auto it = std::find(clickables.begin(), clickables.end(), clickable);
        if (it != clickables.end()) {
            clickables.erase(it);
        }
    }    
    object->Unbind();
}

void StartupState::EnterFocus() {
  glfwSetWindowUserPointer(window, this);
  glfwSetMouseButtonCallback(window, ClickHandler);
}

void StartupState::LeaveFocus() {

}

void StartupState::Update() {
  for (std::shared_ptr<Updatable> object : updatables) {
    object->Update();
  }
  if (ZeldaInformationHandler::GetIsOptionsInitialized()) {
    driver->Engine().PopState();
  }
}

void StartupState::Draw(double time) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(shaders);

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    float x = 800;
    float y = 400;
    float z = y/2;
    float aspectRatio = x/y;
    float fov = glm::pi<float>()/2;
    glm::mat4 Projection = glm::perspective(fov, aspectRatio, 10.0f, z+1.0f);
    glm::mat4 View       = glm::lookAt(glm::vec3(x/2,y/2,z), glm::vec3(x/2,y/2,0), glm::vec3(0,1,0));
    glm::mat4 PV       = Projection * View;
    
    for (auto object : drawables) {
        object->SetCameraTransform(PV);
        object->SetTransformID(transformID);
        object->Draw(time);
    }
    
    glfwSwapBuffers(window);
    glUseProgram(0);    
    glfwPollEvents();
}

void StartupState::ClickHandler(int button, int action, int mods) {
  for (auto clickable : clickables) {
    clickable->ClickHandler(button, action, mods);
  }
}

void StartupState::ClickHandler(GLFWwindow* window, int button, int action, int mods) {
  StartupState* startupState = reinterpret_cast<StartupState *>(glfwGetWindowUserPointer(window));
  startupState->ClickHandler(button, action, mods);
}
