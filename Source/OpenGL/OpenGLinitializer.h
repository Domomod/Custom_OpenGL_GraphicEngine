
//
// Created by dominik on 09.02.19.
//

#ifndef GAMEENGINE_OPENGLINITALIZER_H
#define GAMEENGINE_OPENGLINITALIZER_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/gl3w.h>

#include "Source/OpenGL/Window/Window.h"
#include "Source/OpenGL/ShaderProgram/Shader.h"
#include "Source/MyExceptions.h"
#include "OnChangeListenerTemplate/OnChangeListener.h"
#include "Source/EntitySystem/Mesh.h"
#include "Source/OpenGL/SendingDataToGPU/ToGPUattribueSender.h"
#include "Source/OpenGL/SendingDataToGPU/ToGPUniformSender.h"
#include "Source/OpenGL/RenderSystem/AbstractRenderer.h"

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
};

#endif //GAMEENGINE_APPLICATION_H
