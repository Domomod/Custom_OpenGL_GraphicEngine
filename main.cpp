#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "Window.h"

int main(int argc, char* argv[]) {
    //OpenGL Init
    if(!glfwInit()){
        std::cout << "Failed to initialize GLFW.\n";
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    Window window("Game Engine", 800, 600);
    window.makeCurrent();

    if(gl3wInit()) {
        std::cerr << "Failed to initialize OpenGl.\n";
        return -1;
    }

    std::cout << "\tVendor:\t" << glGetString(GL_VENDOR) << std::endl;
    std::cout << "\tRenderer:\t" << glGetString(GL_RENDERER) << std::endl;
    std::cout << "\tOpenGL version:\t" << glGetString(GL_VERSION) << std::endl;
    std::cout << "\tGLSL version:\t" << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

    glClearColor(1,0,0,0);
    glfwSwapInterval(1);

    while(window.isRunning()){
        window.onRender();
        glfwPollEvents();
    }


    return 0;
}