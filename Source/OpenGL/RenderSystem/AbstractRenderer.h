#include <memory>

//
// Created by dominik on 27.02.19.
//

#ifndef GAMEENGINE_RENDERER_H
#define GAMEENGINE_RENDERER_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Source/OpenGL/ShaderProgram/Shader.h"
#include "Source/EntitySystem/Mesh.h"
#include "Source/OpenGL/Window/Window.h"
#include "Source/OpenGL/SendingDataToGPU/ToGPUattribueSender.h"
#include "Source/OpenGL/SendingDataToGPU/ToGPUniformSender.h"

class AbstractRenderer {
public:
    AbstractRenderer(ToGPUattribueSender* toGPUattribueSender,
                     ToGPUniformSender* toGPUniformSender) : toGPUattribueSender(
            toGPUattribueSender), toGPUniformSender(toGPUniformSender) {}

    virtual ~AbstractRenderer() = default;

    virtual void init();

    virtual void fullRender(Mesh& mesh, Window& window);

    void setFillType(GLenum fillType);

    void setGeometryType(GLenum geometryType);

    OnChangeListener<std::pair<int, int>> &getOnWindowResizeProjectionUpdater();

protected:
    virtual void initialiseUniformSender() = 0;
    virtual void initialiseAttributeSender() = 0;

    virtual void sendMeshDataToGPU(Mesh& mesh) = 0;

    virtual void render(Mesh &mesh) = 0;

    //
    GLenum fillType = GL_LINE;
    GLenum geometryType = GL_TRIANGLES;

    std::string shadersPath = "../Shaders/";

    //Camera
    GLfloat FOV = 45.0;
    GLfloat aspect = 800.f/600.f;
    GLfloat zNear = 1;
    GLfloat zFar = 1000;

    //Communication
    OnChangeListener<std::pair<int,int>> onWindowResizeProjectionUpdater;
    std::shared_ptr<ToGPUattribueSender> toGPUattribueSender;
    std::shared_ptr<ToGPUniformSender> toGPUniformSender;
};


#endif //GAMEENGINE_RENDERER_H
