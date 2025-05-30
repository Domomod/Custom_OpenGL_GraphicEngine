//
// Created by dominik on 17.03.19.
//

#ifndef GAMEENGINE_APPLICATION_H
#define GAMEENGINE_APPLICATION_H

#include <memory>
#include <vector>

#include "Platform/OpenGL/Window/Window.h"
#include "Platform/OpenGL/OpenGLinitializer.h"
#include "Platform/OpenGL/ShaderProgram/Shader.h"
#include "Platform/OpenGL/Window/InputSystem.h"
#include "GraphicsLayer/FreeCamera.h"

#include "EngineLogic/EntitySystem.h"
#include "Assets/ModelLoader.h"
#include "Assets/Textures/TextureLoader.h"

class Application
{
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
    FreeCamera freeCamera;
    EntitySystem entitySystem;
    ModelLoader modelLoader;
    TextureLoader textureLoader;

    OnChangeListener<std::pair<int, int>> windowResizeListener;
    OnChangeListener<KeyInfo> keyActionListener;

    std::shared_ptr<Shader> basicShader = std::make_shared<Shader>();
    std::shared_ptr<Shader> waterShader = std::make_shared<Shader>();
    std::shared_ptr<Shader> texturedShader = std::make_shared<Shader>();
    std::shared_ptr<Shader> animationShader = std::make_shared<Shader>();
    std::shared_ptr<Shader> pbrShader = std::make_shared<Shader>();
    std::shared_ptr<Shader> skyBoxShader = std::make_shared<Shader>();
    std::shared_ptr<Shader> skyBoxFromEquirectangularImageShader = std::make_shared<Shader>();
    std::shared_ptr<Shader> diffuseIrradianceMapShader = std::make_shared<Shader>();

    std::vector<std::shared_ptr<Entity>> entities;
    std::vector<std::shared_ptr<Entity>> entities2;
    std::vector<std::shared_ptr<Entity>> *scene;

    bool shadersCompiled = false;

    glm::mat4 ModelViewProjection;
    glm::mat4 Model;
    glm::mat4 View;
    glm::mat4 Projection;

    float OutsideTesselation = 1.0f;
    float InsideTesselation = 1.0f;

    glm::vec4 center = glm::vec4(0.f, 0.f, -10.f, 1.f);
    float time = 0.0f;
    float amplitude = 0.5f;
    float frequency = 0.4;


};


#endif //GAMEENGINE_APPLICATION_H
