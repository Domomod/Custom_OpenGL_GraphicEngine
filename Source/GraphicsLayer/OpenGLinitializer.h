
//
// Created by dominik on 09.02.19.
//

#ifndef GAMEENGINE_OPENGLINITALIZER_H
#define GAMEENGINE_OPENGLINITALIZER_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <GL/freeglut.h> /*gl3w seems not to have GLAPIENTRY defined*/


class OpenGlInitalizer {
public:
    OpenGlInitalizer() = default;

    virtual ~OpenGlInitalizer() = default;

    void initGLFW();
    void initOpenGL();

    GLint getMaxPatchVertices() const;

private:
    static void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
                                           const GLchar *message, const void *userParam);

    GLint maxPatchVertices;
    GLint maxUniformBufferBindings;
};

#endif //GAMEENGINE_APPLICATION_H
