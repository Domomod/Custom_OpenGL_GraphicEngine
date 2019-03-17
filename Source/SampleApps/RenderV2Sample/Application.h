//
// Created by dominik on 17.03.19.
//

#ifndef GAMEENGINE_APPLICATION_H
#define GAMEENGINE_APPLICATION_H

#include <memory>
#include <vector>

#include "Source/OpenGL/Window/Window.h"
#include "Source/OpenGL/OpenGLinitializer.h"
#include "Source/OpenGL/ShaderProgram/Shader.h"

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



    std::vector<GLushort> indicies = {
            0, 1, 2
    };

    struct InstancedModels{
        std::vector<glm::vec3> positions = {
                glm::vec3(-1,-1,0),
                glm::vec3(0,1,0),
                glm::vec3(1,-1,0)
        };

        std::vector<glm::mat4> ModelMatrixes = {
                glm::mat4(1),
                glm::translate(glm::mat4(1), glm::vec3(-1.f, 0.f, -1.f)),
                glm::translate(glm::mat4(1), glm::vec3(1.f, 1.f, -4.f)),
                glm::translate(glm::mat4(1), glm::vec3(0.f, 0.f, -5.f))
        };

        } instancedModels;

    glm::vec4 color = glm::vec4(1,0,0,1);
};


#endif //GAMEENGINE_APPLICATION_H
