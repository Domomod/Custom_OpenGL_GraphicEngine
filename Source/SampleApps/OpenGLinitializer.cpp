//
// Created by dominik on 09.02.19.
//

#include "OpenGLinitializable.h"

void OpenGlInitalizable::initOpenGL() {
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

void OpenGlInitalizable::MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
                                         const GLchar *message, const void *userParam) {
    fprintf( stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
             ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ),
             type, severity, message );
    if(type == GL_DEBUG_TYPE_ERROR)
        throw new OpenGlException();
}

GLfloat OpenGlInitalizable::getMaxPatchVerticies() const {
    return maxPatchVerticies;
}
