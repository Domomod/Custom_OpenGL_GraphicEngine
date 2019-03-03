//
// Created by dominik on 03.03.19.
//

#ifndef GAMEENGINE_TESSELATIONAPP_H
#define GAMEENGINE_TESSELATIONAPP_H


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/gl3w.h>

#include "Source/OpenGL/Window/Window.h"
#include "Source/OpenGL/OpenGLinitializer.h"
#include "Source/OpenGL/RenderSystem/Forward/ForwardTesselation.h"
#include "Source/EntitySystem/MeshGenerator.h"

class TesselationApp {
private:
    std::shared_ptr<Window> window;
    ForwardTesselation renderer;
    OpenGlInitalizer openGlInitalizer;
    Mesh mesh;
public:
    TesselationApp(){
        openGlInitalizer.initGLFW();
        window = std::make_shared<Window>();
        window->getResizeNotifierPtr()->addListener(&renderer.getOnWindowResizeProjectionUpdater());
        window->makeCurrent();
        openGlInitalizer.initOpenGL();
        mesh = MeshGenerator::generateSymetricalRectanuglarMesh(150, 150, 25.f, 25.f, 0.f, -1.f, -5.f);
        renderer.init();
    }
    virtual ~TesselationApp() = default;

    void run(){
        renderer.sendMeshDataToGPU(mesh);
        while(window->isRunning()){
            renderer.render(mesh);
            window->swapBuffers();
            glfwPollEvents();
        }
    }
};


#endif //GAMEENGINE_TESSELATIONAPP_H
