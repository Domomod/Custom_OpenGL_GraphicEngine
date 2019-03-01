//
// Created by dominik on 09.02.19.
//

#include "Application.h"
#include "MeshGenerator.h"
#include "Mesh.h"

#include <glm/gtc/type_ptr.hpp>


Application::Application() : mainWindow("Game Engine", 800, 600) {

}


Application::~Application() {
}


void Application::start() {
    //There must be a current window before OpenGL initialisation
    mainWindow.makeCurrent();

    initialiseOpenGL();
    renderer.initialize();
    initialiseCommunication();
    mesh = MeshGenerator::generateSymetricalRectanuglarMesh(2, 2, 3.f, 3.f, center.x, center.y,
                                                            center.z);
    mainLoop();
}

void Application::initialiseOpenGL() {
    if(gl3wInit()) {
        std::cerr << "Failed to initialize OpenGl.\n";
        throw GlfwInitalisationFailedException();
    }

    std::cout << "\tVendor:\t" << glGetString(GL_VENDOR) << std::endl;
    std::cout << "\tRenderer:\t" << glGetString(GL_RENDERER) << std::endl;
    std::cout << "\tOpenGL version:\t" << glGetString(GL_VERSION) << std::endl;
    std::cout << "\tGLSL version:\t" << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    glGetIntegerv(GL_MAX_PATCH_VERTICES, &maxPatchVerticies);
    std::cout << "\tTesselation Shader: Max supported patch verticies " <<  maxPatchVerticies << std::endl;

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(MessageCallback, 0);

    glClearColor(0.1,0.1,0.1,0);
    glfwSwapInterval(1);
}

void Application::initialiseCommunication() {
    mainWindow.getResizeNotifierPtr()->addListener(&(renderer.getOnWindowResizeProjectionUpdater()));
}


void Application::mainLoop() {
    renderer.sendMeshDataToGPU(mesh);
    while(mainWindow.isRunning()){
        renderer.render(mesh, mainWindow);
        glfwPollEvents();
    }
}

void Application::MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
                                  const GLchar *message, const void *userParam) {
    fprintf( stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
             ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ),
             type, severity, message );
    if(type == GL_DEBUG_TYPE_ERROR)
        throw new OpenGlException();
}
