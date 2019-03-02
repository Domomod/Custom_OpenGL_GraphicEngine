#include <memory>

//
// Created by dominik on 27.02.19.
//

#ifndef GAMEENGINE_RENDERER_H
#define GAMEENGINE_RENDERER_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../View/Shader.h"
#include "Mesh.h"
#include "../View/Window.h"
#include "ToGPUattribueSender.h"
#include "ToGPUniformSender.h"

class AbstractRenderer {
protected:
    GLenum fillType;
    GLenum geometryType;

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

public:
    AbstractRenderer();

    virtual ~AbstractRenderer();

    virtual void init() {
        initializeGLenums();
        initialiseAttributeSender();
        initialiseUniformSender();

    }

    virtual void initializeGLenums() = 0;
    virtual void initialiseUniformSender() = 0;
    virtual void initialiseAttributeSender() = 0;

    virtual void sendMeshDataToGPU(Mesh& mesh) = 0;

    virtual void render(Mesh& mesh, Window& window) = 0;

    virtual void fullRender(Mesh& mesh, Window& window){
        sendMeshDataToGPU(mesh);

    };

    OnChangeListener<std::pair<int, int>> &getOnWindowResizeProjectionUpdater() {
        return onWindowResizeProjectionUpdater;
    }
};


#endif //GAMEENGINE_RENDERER_H
