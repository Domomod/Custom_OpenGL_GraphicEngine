#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>

#include "SampleApps/Application.h"
#include "MyExceptions.h"

int main(int argc, char* argv[]) {
    //GLFW Initialisation code, must be completed before window constructor
    if(!glfwInit()){
        std::cout << "Failed to initialize GLFW.\n";
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //Application code
    Application application;
    application.start();

    return 0;
}