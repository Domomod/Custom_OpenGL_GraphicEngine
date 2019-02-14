
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

    //Math
    glm::mat4 Projection = glm::mat4(1);
    glm::mat4 ModelView = glm::mat4(1);

    void mainLoop(){
        while(mainWindow.isRunning()){
            mainWindow.onRender();
            glfwPollEvents();
        }
    }

public:
    OnChangeListener<std::pair<int,int>> onWindowResizeProjectionUpdater;

    Application() : indicies{0,1,2}, mainWindow("Game Engine", 800, 600) {
        verticies[0].position = glm::vec3(-1,-1,0);
        verticies[1].position = glm::vec3(0,1,0);
        verticies[2].position = glm::vec3(0,-1,0);

        verticies[0].color = glm::vec3(1,0,0);
        verticies[1].color = glm::vec3(0,1,0);
        verticies[2].color = glm::vec3(0,0,1);

        onWindowResizeProjectionUpdater.setReactionFuncPtr([&](std::pair<int,int> newWidthHeight){
            double width = newWidthHeight.first;
            double height = newWidthHeight.second;
            Projection = glm::ortho(0.0, width, 0.0, height, 1.0, -1.0);
        });
    }

    virtual ~Application(){
        mainShader.deleteProgram();
    }

    void start(){
        //There must be a current window before OpenGL initialisation
        mainWindow.makeCurrent();
        mainWindow.getResizeNotifier().addListener(&onWindowResizeProjectionUpdater);

        //OpenGL initialisation
        if(gl3wInit()) {
            std::cerr << "Failed to initialize OpenGl.\n";
            throw GlfwInitalisationFailedException();
        }

        std::cout << "\tVendor:\t" << glGetString(GL_VENDOR) << std::endl;
        std::cout << "\tRenderer:\t" << glGetString(GL_RENDERER) << std::endl;
        std::cout << "\tOpenGL version:\t" << glGetString(GL_VERSION) << std::endl;
        std::cout << "\tGLSL version:\t" << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

        glClearColor(1,0,0,0);
        glfwSwapInterval(1);

        //Shader loading and consolidation
        try {
            mainShader.loadFromFile(GL_VERTEX_SHADER, shadersPath + "basic.vert");
            mainShader.loadFromFile(GL_FRAGMENT_SHADER, shadersPath + "basic.frag");
            mainShader.createAndLinkProgram();
            mainShader.use();
            mainShader.addAttribute("vertex");
            mainShader.addUniform("ModelViewProjection");
            mainShader.unuse();
        } catch( MyException& e) {
            std::cerr << e.getType() << ":\n" << e.getMessage();
            throw e;
        }
        //TODO: ask for proper path if sth goes wrong

        mainLoop();
    }
};


#endif //GAMEENGINE_APPLICATION_H
