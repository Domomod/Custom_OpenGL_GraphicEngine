#include <memory>

//
// Created by dominik on 27.02.19.
//

#ifndef GAMEENGINE_FORWARDTESSELATIONRENDERER_H
#define GAMEENGINE_FORWARDTESSELATIONRENDERER_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Source/OpenGL/ShaderProgram/Shader.h"
#include "Source/EntitySystem/Mesh.h"
#include "Source/OpenGL/Window/Window.h"
#include "Source/OpenGL/SendingDataToGPU/ToGPUattribueSender.h"
#include "Source/OpenGL/SendingDataToGPU/ToGPUniformSender.h"
#include "ForwardAbstractRenderer.h"
#include "Source/OpenGL/OpenGLinitializer.h"

class ForwardSinWaveRenderer : public ForwardAbstractRenderer {
private:
    //Uniforms
    glm::mat4 Projection = glm::mat4(1);
    glm::mat4 ModelView = glm::mat4(1);
    glm::mat4 ModelViewProjection = glm::mat4(1);

    GLfloat time = 1.0f;
    glm::vec3 center = glm::vec3(0.f, -0.5f, -3.f);
public:
    ForwardSinWaveRenderer() = default;

    ~ForwardSinWaveRenderer() override = default;

    void setTime(GLfloat time);

    void setCenter(const glm::vec3 &center);

protected:
    void initShaders() override {
        try {
            shader->loadFromFile(Shader::VERTEX, shadersPath + "sinWave.vs");
            shader->loadFromFile(Shader::FRAGMENT, shadersPath + "basic.fs");
            shader->createAndLinkProgram();
            shader->use();
            shader->addAttribute("position");
            shader->addAttribute("color");
            shader->addUniform("ModelViewProjection");
            shader->addUniform("time");
            shader->addUniform("center");
            shader->unuse();
        } catch( MyException& e) {
            std::cerr << e.getType() << ":\n" << e.getMessage();
            throw e;
            //TODO: ask for proper path if sth goes wrong
        }
    };

public:

    void init() override {
        ForwardAbstractRenderer::init();

        Projection = glm::perspective(FOV, aspect, zNear, zFar);
        ModelViewProjection = Projection * ModelView;

        onWindowResizeProjectionUpdater.setReactionFuncPtr([&](std::pair<int,int> newWidthHeight){
            int width = newWidthHeight.first;
            int height = newWidthHeight.second;
            aspect = width / height;
            Projection = glm::perspective(FOV, aspect, zNear, zFar);
        });
    }

    void initialiseUniformSender() override {
        toGPUniformSender->addShader(shader);

        toGPUniformSender->addUniform(shader,
                                     UniformSendingInfo::MatrixType::FOUR,
                                     glm::value_ptr(ModelViewProjection),
                                     shader->getUniform("ModelViewProjection"));

        toGPUniformSender->addUniform(shader,
                                      UniformSendingInfo::UniformType::UNIFORM_FLOAT,
                                      &time,
                                      shader->getUniform("time"),
                                      1);

        toGPUniformSender->addUniform(shader,
                                      UniformSendingInfo::UniformType::UNIFORM_FLOAT,
                                      &center,
                                      shader->getUniform("center"),
                                      3);

    }

    void initialiseAttributeSender() override {
        toGPUattribueSender->createBuffers();

        toGPUattribueSender->addShader(shader);

        toGPUattribueSender->addAttribute(shader,
                                         shader->getAttribute("position"),
                                         3,
                                         GL_FLOAT,
                                         Vertex::getPositionOffset()
        );

        toGPUattribueSender->addAttribute(shader,
                                         shader->getAttribute("color"),
                                         3,
                                         GL_FLOAT,
                                         Vertex::getColorOffset()
        );
    }

    void sendMeshDataToGPU(Mesh& mesh) override {
        toGPUattribueSender->sendGeometryAndTopology(shader,mesh);
    }

    void render(Mesh &mesh) override;
};


#endif //GAMEENGINE_RENDERER_H
