
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


class OpenGlInitalizer
{
public:
    OpenGlInitalizer()
    {
        initOpenGL();
    };

    virtual ~OpenGlInitalizer() = default;

    static void initGLFW();

    void initOpenGL();

    GLint getMaxPatchVertices() const;

private:
    static void APIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
                                         const GLchar *message, const void *userParam);

    GLint maxPatchVertices;
    GLint maxUniformBufferBindings;
};

#endif //GAMEENGINE_APPLICATION_H
