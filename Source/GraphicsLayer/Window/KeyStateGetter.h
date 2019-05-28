//
// Created by dominik on 28.05.19.
//

#ifndef GAMEENGINE_KEYSTATEGETTER_H
#define GAMEENGINE_KEYSTATEGETTER_H

#include <GLFW/glfw3.h>

class KeyStateGetter{
    friend class WindowInputSystem;
public:
    inline int getKey(int key) { return glfwGetKey(myWindow, key); }
private:
    KeyStateGetter() = default;
    inline void setWindow(GLFWwindow *myWindow) {this->myWindow = myWindow;}
    GLFWwindow* myWindow;
};


#endif //GAMEENGINE_KEYSTATEGETTER_H
