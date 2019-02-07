#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <iostream>


const int WIDTH = 1280;
const int HEIGHT = 960;

void OnShutdown(){

}

void OnResize(int newWidth, int newHeight){

}

void OnRender(){
}

void OnInit(){

}

int main(int argc, char* argv[]) {
    //OpenGL Init
    if(!glfwInit()){
        std::cout << "Failed to initialize GLFW.\n";
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    auto window = glfwCreateWindow(800, 400, "My window", nullptr, nullptr);
    if(!window){
        return -1;
    }

    glfwMakeContextCurrent(window);
    if(gl3wInit()) {
        std::cerr << "Failed to initialize OpenGl.\n";
        return -1;
    }

    std::cout << "\tVendor:\t" << glGetString(GL_VENDOR) << std::endl;
    std::cout << "\tRenderer:\t" << glGetString(GL_RENDERER) << std::endl;
    std::cout << "\tOpenGL version:\t" << glGetString(GL_VERSION) << std::endl;
    std::cout << "\tGLSL version:\t" << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

    //OnInit();

    return 0;
}