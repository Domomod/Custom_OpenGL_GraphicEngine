//
// Created by dominik on 09.02.19.
//

#include "OpenGLinitializer.h"
#include "Source/MyExceptions.h"

void OpenGlInitalizer::initGLFW() {
    if(!glfwInit()){
        std::cout << "Failed to initialize GLFW.\n";
        throw GlfwInitalisationFailedException();
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void OpenGlInitalizer::initOpenGL() {

    if(gl3wInit()) {
        std::cerr << "Failed to initialize OpenGl.\n";
        throw GlfwInitalisationFailedException("Tip: Are you sure there is a current Window (last window to call makeCurrent() method )?");
    }

    std::cout << "\tVendor:\t" << glGetString(GL_VENDOR) << std::endl;
    std::cout << "\tRenderer:\t" << glGetString(GL_RENDERER) << std::endl;
    std::cout << "\tOpenGL version:\t" << glGetString(GL_VERSION) << std::endl;
    std::cout << "\tGLSL version:\t" << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    glGetIntegerv(GL_MAX_PATCH_VERTICES, &maxPatchVertices);
    glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, &maxUniformBufferBindings);
    std::cout << "\tMax supported patch verticies " <<  maxPatchVertices << std::endl;
    std::cout << "\tMax supported uniform buffer bindings " <<  maxUniformBufferBindings << std::endl;

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(MessageCallback, 0);

    glClearColor(0.1,0.1,0.1,0);
    glfwSwapInterval(1);
}

void OpenGlInitalizer::MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
                                         const GLchar *message, const void *userParam) {
    fprintf( stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
             ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ),
             type, severity, message );
    const char * msg = message;
    if(type == GL_DEBUG_TYPE_ERROR)
        throw OpenGlException(msg);
}

GLint OpenGlInitalizer::getMaxPatchVertices() const {
    return maxPatchVertices;
}