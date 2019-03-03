
//
// Created by dominik on 09.02.19.
//

#ifndef GAMEENGINE_OPENGLINITALIZABLE_H
#define GAMEENGINE_OPENGLINITALIZABLE_H

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
#include "AbstractRenderer.h"

class OpenGlInitalizable {
protected:
    void initOpenGL();

public:
    GLfloat getMaxPatchVerticies() const;

private:
    GLfloat maxPatchVerticies;

    void MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
                                              const GLchar *message, const void *userParam);;


#endif //GAMEENGINE_APPLICATION_H
