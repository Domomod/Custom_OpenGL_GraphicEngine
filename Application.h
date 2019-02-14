
//
// Created by dominik on 09.02.19.
//

#ifndef GAMEENGINE_APPLICATION_H
#define GAMEENGINE_APPLICATION_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/gl3w.h>

#include "Window.h"
#include "Shader.h"
#include "MyExceptions.h"
#include "OnChangeListenerTemplate/OnChangeListener.h"

class Application {
public:
    Application();

    virtual ~Application();

    void start();

private:
    //Object data
    class Vertex{
    public:
        glm::vec3 color;
        glm::vec3 position;
    };
    Vertex verticies[3];
    GLushort indicies[3];

    //View
    Window mainWindow;
    Shader mainShader;

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
};


#endif //GAMEENGINE_APPLICATION_H
