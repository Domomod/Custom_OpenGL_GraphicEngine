
//
// Created by dominik on 09.02.19.
//

#ifndef GAMEENGINE_SINWAVEAPP_H
#define GAMEENGINE_SINWAVEAPP_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/gl3w.h>

#include "../../View/Window.h"
#include "../OpenGLinitializer.h"
#include "ForwardSinWaveRenderer.h"
#include "../MeshGenerator.h"
class SinWaveApp {
private:
    std::shared_ptr<Window> window;
    ForwardSinWaveRenderer renderer;
    OpenGlInitalizer openGlInitalizer;
    Mesh mesh;
public:
    SinWaveApp(){
        openGlInitalizer.initGLFW();
        window = std::make_shared<Window>();
        window->getResizeNotifierPtr()->addListener(&renderer.getOnWindowResizeProjectionUpdater());
        window->makeCurrent();
        openGlInitalizer.initOpenGL();
        mesh = MeshGenerator::generateSymetricalRectanuglarMesh(150, 150, 25.f, 25.f, 0.f, -1.f, -5.f);
        renderer.init();
    }
    virtual ~SinWaveApp() = default;

    void run(){
        renderer.sendMeshDataToGPU(mesh);
        renderer.setCenter(glm::vec3(0.f, -0.7f, -5.f));
        while(window->isRunning()){
            renderer.setTime(static_cast<GLfloat>(glfwGetTime()));
            renderer.render(mesh);
            window->swapBuffers();
        }
    }
};


#endif //GAMEENGINE_SINWAVEAPP_H
