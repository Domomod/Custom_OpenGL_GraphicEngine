//
// Created by dominik on 08.02.19.
//

#ifndef GAMEENGINE_WINDOW_H
#define GAMEENGINE_WINDOW_H

#include <GLFW/glfw3.h>
#include <iostream>
#include <memory>

class Window {



private:
    GLFWwindow* window;

    static void onShutdown(GLFWwindow* window);

    static void onResize(GLFWwindow* window, int newWidth, int newHeight);

public:
    explicit Window(std::string title = "Window", int width = 800, int height = 400);

    virtual ~Window();

    void onRender();

    void makeCurrent();

    bool isRunning();

    Window(const Window&) = delete;
};


#endif //GAMEENGINE_WINDOW_H
