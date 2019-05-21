//
// Created by dominik on 07.03.19.
//

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>

#include "Source/GraphicsLayer/OpenGLinitializer.h"

#include "Application.h"

int main(){
    OpenGlInitalizer::initGLFW();
    Application app;
    app.main();
}