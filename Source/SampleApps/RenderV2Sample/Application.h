//
// Created by dominik on 17.03.19.
//

#ifndef GAMEENGINE_APPLICATION_H
#define GAMEENGINE_APPLICATION_H

#include <memory>
#include <vector>

#include "Source/GraphicsLayer/Window/Window.h"
#include "Source/GraphicsLayer/OpenGLinitializer.h"
#include "Source/GraphicsLayer/ShaderProgram/Shader.h"
#include "Source/GraphicsLayer/Window/InputSystem.h"
#include "Source/GraphicsLayer/FreeCamera.h"

#include "Source/DataLayer/StorageManagment/EntitySystem.h"

class Application {
public:
    Application();

    virtual ~Application();

    void main();

private:
    std::shared_ptr<OpenGlInitalizer> openGlInitalizer;
    std::shared_ptr<Window> window;
    std::shared_ptr<WindowInputSystem> windowInputSystem;
    std::shared_ptr<FreeCamera> freeCamera;

    std::shared_ptr<Shader> shader;
    std::shared_ptr<Shader> waterShader;
    std::shared_ptr<Shader> texturedShader;
    std::shared_ptr<Shader> animationShader;

    glm::mat4 ModelViewProjection;
    glm::mat4 View;
    glm::mat4 Projection;

    float OutsideTesselation = 1.0f;
    float InsideTesselation = 1.0f;

    glm::vec4 center = glm::vec4(0.f,0.f,-10.f,1.f);
    float time = 0.0f;
    float amplitude = 0.5f;
    float frequency = 0.4;

    OnChangeListener<std::pair<int,int>> windowResizeListener;

    EntitySystem entitySystem;
};


#endif //GAMEENGINE_APPLICATION_H
