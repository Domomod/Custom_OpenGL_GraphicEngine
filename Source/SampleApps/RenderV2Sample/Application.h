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

#include "Source/DataLayer/StorageManagment/EntitySystem.h"

class Application {
public:
    Application();

    virtual ~Application();

    void main();

private:
    std::shared_ptr<OpenGlInitalizer> openGlInitalizer;
    std::shared_ptr<Window> window;
    std::shared_ptr<Shader> shader;

    glm::mat4 ModelViewProjection;
    glm::mat4 Model;
    glm::mat4 View;
    glm::mat4 Projection;
    OnChangeListener<std::pair<int,int>> windowResizeListener;

    EntitySystem entitySystem;
};


#endif //GAMEENGINE_APPLICATION_H
