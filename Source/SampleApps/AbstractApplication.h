//
// Created by dominik on 26.02.19.
//

#ifndef GAMEENGINE_ABSTRACTAPPLICATION_H
#define GAMEENGINE_ABSTRACTAPPLICATION_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/gl3w.h>

#include "../View/Window.h"
#include "../View/Shader.h"
#include "../MyExceptions.h"
#include "OnChangeListener.h"
#include "Mesh.h"

class AbstractApplication {
public:
    AbstractApplication();

    virtual ~AbstractApplication();

    void start();
private:
    //View
    Window mainWindow;


    Renderer renderer;

    ToGPUdataLoader toGPUdataLoader;

    void mainLoop();
    void createBuffers();

    void insertGeometryAndTopologyIntoBuffers();
    void Render();

    static void GLAPIENTRY
    MessageCallback( GLenum source,
                     GLenum type,
                     GLuint id,
                     GLenum severity,
                     GLsizei length,
                     const GLchar* message,
                     const void* userParam );
};


#endif //GAMEENGINE_ABSTRACTAPPLICATION_H
