
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

class Application {
public:
    Application();

    virtual ~Application();

    void start();
private:
    //Scene
    Mesh mesh;

    //View
    Window mainWindow;
    Shader mainShader;

    GLfloat FOV = 45.0;
    GLfloat aspect = 800.f/600.f;
    GLfloat zNear = 1;
    GLfloat zFar = 1000;

    std::string shadersPath = "../Shaders/";

    GLuint vaoId;
    GLuint vboVerticesId;
    GLuint vboIndiciesId;

    //Communication
    OnChangeListener<std::pair<int,int>> onWindowResizeProjectionUpdater;

    //Math
    glm::mat4 Projection = glm::mat4(1);
    glm::mat4 ModelView = glm::mat4(1);


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


#endif //GAMEENGINE_APPLICATION_H
