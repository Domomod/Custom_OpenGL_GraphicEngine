
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
    std::shared_ptr<Shader> mainShader;

    GLfloat FOV = 45.0;
    GLfloat aspect = 800.f/600.f;
    GLfloat zNear = 1;
    GLfloat zFar = 1000;

    std::string shadersPath = "../Shaders/";

    GLint maxPatchVerticies = 0;

    //Communication
    OnChangeListener<std::pair<int,int>> onWindowResizeProjectionUpdater;

    ToGPUattribueSender toGPUattribueSender; //TODO: Constructor bugged

    ToGPUniformSender toGPUniformSender;

    //Math
    glm::mat4 Projection = glm::mat4(1);
    glm::mat4 ModelView = glm::mat4(1);

    glm::mat4 ModelViewProjection;

    GLfloat OuterTesselationLevel = 1.0f;
    GLfloat InnerTesselationLevel = 1.0f;

    void mainLoop();

    void initialiseGPUSenders();
    void initialiseUniformSender();
    void initialiseAttributeSender();

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
