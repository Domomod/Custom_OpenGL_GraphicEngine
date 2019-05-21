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
    void loadShaders();
    void freeResources();

    Window window;
    OpenGlInitalizer openGlInitalizer;

    WindowInputSystem windowInputSystem;
    FreeCamera        freeCamera;
    EntitySystem entitySystem;

    OnChangeListener<std::pair<int,int>> windowResizeListener;
    OnChangeListener<char*> applicatonKeyboardStateListener;

    std::shared_ptr<Shader> basicShader;
    std::shared_ptr<Shader> waterShader;
    std::shared_ptr<Shader> texturedShader;
    std::shared_ptr<Shader> animationShader;
    std::shared_ptr<Shader> pbrShader;
    std::shared_ptr<Shader> skyBoxShader;
    std::shared_ptr<Shader> equirSkyBoxShader;
    std::shared_ptr<Shader> equirToSkyboxShader;

    bool shadersCompiled = false;

    glm::mat4 ModelViewProjection;
    glm::mat4 Model;
    glm::mat4 View;
    glm::mat4 Projection;

    float OutsideTesselation = 1.0f;
    float InsideTesselation = 1.0f;

    glm::vec4 center = glm::vec4(0.f,0.f,-10.f,1.f);
    float time = 0.0f;
    float amplitude = 0.5f;
    float frequency = 0.4;



};


#endif //GAMEENGINE_APPLICATION_H
