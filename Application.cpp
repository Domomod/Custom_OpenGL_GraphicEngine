//
// Created by dominik on 09.02.19.
//

#include "Application.h"

#include <GL/freeglut.h>
#include <glm/gtc/type_ptr.hpp>


Application::Application() : indicies{0,1,2}, mainWindow("Game Engine", 800, 600) {
    verticies[0].position = glm::vec3(-1,-1,0);
    verticies[1].position = glm::vec3(0,1,0);
    verticies[2].position = glm::vec3(0,-1,0);

    verticies[0].color = glm::vec3(1,0,0);
    verticies[1].color = glm::vec3(0,1,0);
    verticies[2].color = glm::vec3(0,0,1);

    onWindowResizeProjectionUpdater.setReactionFuncPtr([&](std::pair<int,int> newWidthHeight){
        Projection = glm::ortho(-1.0, 1.0, -1.0, 1.0);
    });
}


Application::~Application() {
    mainShader.deleteProgram();
    glDeleteBuffers(1, &vboIndiciesId);
    glDeleteBuffers(1, &vboVerticesId);
    glDeleteVertexArrays(1, &vaoId);
}


void Application::start() {
    //There must be a current window before OpenGL initialisation
    mainWindow.makeCurrent();
    mainWindow.getResizeNotifierPtr()->addListener(&onWindowResizeProjectionUpdater);

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
        mainShader.loadFromFile(Shader::VERTEX, shadersPath + "basic.vert");
        mainShader.loadFromFile(Shader::FRAGMENT, shadersPath + "basic.frag");
        mainShader.createAndLinkProgram();
        mainShader.use();
        mainShader.addAttribute("position");
        mainShader.addAttribute("color");
        mainShader.addUniform("ModelViewProjection");
        mainShader.unuse();
    } catch( MyException& e) {
        std::cerr << e.getType() << ":\n" << e.getMessage();
        throw e;
    }
    //TODO: ask for proper path if sth goes wrong

    mainLoop();
}


void Application::mainLoop() {
    createBuffers();
    insertGeometryAndTopologyIntoBuffers();
    while(mainWindow.isRunning()){
        Render();
        glfwPollEvents();
    }
}

void Application::createBuffers() {
    glGenVertexArrays(1, &vaoId);
    glGenBuffers(1, &vboVerticesId);
    glGenBuffers(1, &vboIndiciesId);
}

void Application::insertGeometryAndTopologyIntoBuffers() {
    GLsizei stride = sizeof(Vertex);

    size_t indicesSize = sizeof(indicies);
    size_t verticesSize = sizeof(verticies);
    size_t colorOffset = offsetof(Vertex, color);

    GLuint positionLocation = mainShader.getAttribute("position");
    GLuint colorLocation = mainShader.getAttribute("color");
    GLuint numElements = verticesSize / sizeof(Vertex);


    glBindVertexArray(vaoId);

    glBindBuffer(GL_ARRAY_BUFFER, vboVerticesId);
    glBufferData(GL_ARRAY_BUFFER, verticesSize, verticies, GL_STATIC_DRAW);
    glEnableVertexAttribArray(positionLocation);
    glVertexAttribPointer(positionLocation, numElements, GL_FLOAT, GL_FALSE, stride, 0);

    glBufferData(GL_ARRAY_BUFFER, verticesSize, verticies, GL_STATIC_DRAW);
    glEnableVertexAttribArray(colorLocation);
    glVertexAttribPointer(colorLocation, numElements, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(colorOffset));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndiciesId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, indicies, GL_STATIC_DRAW);

}

void Application::Render() {
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    mainShader.use();

    size_t verticesSize = sizeof(verticies);
    GLuint numElements = verticesSize / sizeof(Vertex);
    GLboolean transpose = GL_FALSE;
    glUniformMatrix4fv(mainShader.getUniform("ModelViewProjection"), 1, transpose, glm::value_ptr(Projection * ModelView));
    glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_SHORT, 0);

    mainShader.unuse();
    mainWindow.swapBuffers();
}


