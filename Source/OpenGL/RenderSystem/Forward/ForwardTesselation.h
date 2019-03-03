//
// Created by dominik on 03.03.19.
//

#ifndef GAMEENGINE_FORWARDTESSELATION_H
#define GAMEENGINE_FORWARDTESSELATION_H

#include "Source/OpenGL/RenderSystem/Forward/ForwardRenderer.h"

class ForwardTesselation : public ForwardRenderer{
private:
    //Uniforms
    glm::mat4 Projection = glm::mat4(1);
    glm::mat4 ModelView = glm::mat4(1);
    glm::mat4 ModelViewProjection = glm::mat4(1);

    GLfloat OuterTesselationLevel = 3.0f;
    GLfloat InnerTesselationLevel = 3.0f;
public:
    ForwardTesselation() {}

    virtual ~ForwardTesselation() {
    }

    void setOuterTesselationLevel(GLfloat OuterTesselationLevel) {
        ForwardTesselation::OuterTesselationLevel = OuterTesselationLevel;
    }

    void setInnerTesselationLevel(GLfloat InnerTesselationLevel) {
        ForwardTesselation::InnerTesselationLevel = InnerTesselationLevel;
    }

protected:
    void initShaders() override {
        try {
            shader->loadFromFile(Shader::VERTEX, shadersPath + "divide.vs");
            shader->loadFromFile(Shader::TESSELATION_CONTROL, shadersPath + "divide.tcs");
            shader->loadFromFile(Shader::TESSELATION_EVALUATION, shadersPath + "divide.tes");
            shader->loadFromFile(Shader::FRAGMENT, shadersPath + "divide.fs");
            shader->createAndLinkProgram();
            shader->use();
            shader->addAttribute("position");
            shader->addAttribute("color");
            shader->addUniform("ModelViewProjection");
            shader->addUniform("OuterTesselationLevel");
            shader->addUniform("InnerTesselationLevel");
            shader->unuse();
        } catch( MyException& e) {
            std::cerr << e.getType() << ":\n" << e.getMessage();
            throw e;
            //TODO: ask for proper path if sth goes wrong
        }
    };

public:

    void init() override {
        ForwardRenderer::init();

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
                                      &OuterTesselationLevel,
                                      shader->getUniform("OuterTesselationLevel"),
                                      1);

        toGPUniformSender->addUniform(shader,
                                      UniformSendingInfo::UniformType::UNIFORM_FLOAT,
                                      &InnerTesselationLevel,
                                      shader->getUniform("InnerTesselationLevel"),
                                      1);

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

    void render(Mesh &mesh) override {
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        shader->use();

        toGPUniformSender->sendUniforms(shader);

        GLuint numIndicies = mesh.getNumberIndicies();

        glPatchParameteri(GL_PATCH_VERTICES, 3);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawElements(GL_PATCHES, numIndicies, GL_UNSIGNED_SHORT, nullptr);

        shader->unuse();
    };
};


#endif //GAMEENGINE_FORWARDTESSELATION_H
