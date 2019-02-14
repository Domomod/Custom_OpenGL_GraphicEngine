//
// Created by dominik on 08.02.19.
//

#include "Window.h"

Window::Window(std::string title, int width, int height) {
    window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if(!window){
        throw std::string("Creating window failed");
    }

    glfwSetWindowSizeCallback(window, onResize);
    glfwSetWindowCloseCallback(window, onShutdown);
}


Window::~Window() {
    glfwDestroyWindow(window);
}


void Window::onShutdown(GLFWwindow *window) {
    std::cout << "See you soon! :)\n";
    glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void Window::onResize(GLFWwindow *window, int newWidth, int newHeight) {
    glViewport(0,0, static_cast<GLsizei>(newWidth), static_cast<GLsizei>(newHeight));
    Window::onWindowResizeNotifiersMap.at(window).notifyListeners(std::make_pair(newWidth, newHeight));
}


void Window::onRender() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glfwSwapBuffers(window);
}


void Window::makeCurrent() {
    glfwMakeContextCurrent(window);
}


bool Window::isRunning() {
    return !glfwWindowShouldClose(window);
}


OnChangeNotifier<std::pair<int,int>>& Window::getResizeNotifier() {
    return onWindowResizeNotifiersMap.at(window);
}