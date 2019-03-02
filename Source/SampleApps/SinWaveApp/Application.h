
//
// Created by dominik on 09.02.19.
//

#ifndef GAMEENGINE_APPLICATION_H
#define GAMEENGINE_APPLICATION_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/gl3w.h>

#include "../View/Window.h"
#include "../View/Shader.h"
#include "../MyExceptions.h"
#include "OnChangeListener.h"
#include "Mesh.h"
#include "ToGPUattribueSender.h"
#include "ToGPUniformSender.h"
#include "ForwardTesselationRenderer.h"

class Application {
public:
    Application();

    virtual ~Application();

    void start();
private:
    //Scene
    Mesh mesh;
    glm::vec3 center = glm::vec3(0.f, -0.5f, -3.f);

    //View
    Window mainWindow;
    Renderer renderer;
    GLint maxPatchVerticies = 0;

    void initialiseOpenGL();
    void initialiseCommunication();

    void mainLoop();

    static void GLAPIENTRY
    MessageCallback( GLenum source,
                     GLenum type,
                     GLuint id,
                     GLenum severity,
                     GLsizei length,
                     const GLchar* message,
                     const void* userParam );
};


#endif //GAMEENGINE_APPLICATION_H
