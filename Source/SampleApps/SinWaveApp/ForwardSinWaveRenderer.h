#include <memory>

//
// Created by dominik on 27.02.19.
//

#ifndef GAMEENGINE_RENDERER_H
#define GAMEENGINE_RENDERER_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../../View/Shader.h"
#include "../Mesh.h"
#include "../../View/Window.h"
#include "../ToGPUattribueSender.h"
#include "../ToGPUniformSender.h"
#include "../ForwardRenderer.h"

class ForwardTesselationRenderer : public ForwardRenderer {
private:
    //Uniforms
    //Math
    glm::mat4 Projection = glm::mat4(1);
    glm::mat4 ModelView = glm::mat4(1);
    glm::mat4 ModelViewProjection = glm::mat4(1);

    GLfloat OuterTesselationLevel = 1.0f;
    GLfloat InnerTesselationLevel = 1.0f;
public:
    ForwardTesselationRenderer();

    virtual ~ForwardTesselationRenderer();

    void initialize(){
        Projection = glm::perspective(FOV, aspect, zNear, zFar);
        ModelViewProjection = Projection * ModelView;

        initializeShaders();
        initialiseGPUSenders();

        onWindowResizeProjectionUpdater.setReactionFuncPtr([&](std::pair<int,int> newWidthHeight){
            int width = newWidthHeight.first;
            int height = newWidthHeight.second;
            aspect = width / height;
            Projection = glm::perspective(FOV, aspect, zNear, zFar);
        });
    }

    virtual void initializeShaders(){
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
    }

    void initialiseGPUSenders(){
        initialiseAttributeSender();
        initialiseUniformSender();
    }

    void initialiseUniformSender(){
        toGPUniformSender.addShader(shader);

        toGPUniformSender.addUniform(shader,
                                     UniformSendingInfo::MatrixType::FOUR,
                                     glm::value_ptr(ModelViewProjection),
                                     shader->getUniform("ModelViewProjection"));

        toGPUniformSender.addUniform(shader,
                                     UniformSendingInfo::UniformType::UNIFORM_FLOAT,
                                     &OuterTesselationLevel,
                                     shader->getUniform("OuterTesselationLevel"),
                                     1);

        toGPUniformSender.addUniform(shader,
                                     UniformSendingInfo::UniformType::UNIFORM_FLOAT,
                                     &InnerTesselationLevel,
                                     shader->getUniform("InnerTesselationLevel"),
                                     1);
    }

    void initialiseAttributeSender(){
        toGPUattribueSender.createBuffers();

        toGPUattribueSender.addShader(shader);

        toGPUattribueSender.addAttribute(shader,
                                         shader->getAttribute("position"),
                                         3,
                                         GL_FLOAT,
                                         Vertex::getPositionOffset()
        );

        toGPUattribueSender.addAttribute(shader,
                                         shader->getAttribute("color"),
                                         3,
                                         GL_FLOAT,
                                         Vertex::getColorOffset()
        );
    }

    void sendMeshDataToGPU(Mesh& mesh){
        toGPUattribueSender.sendGeometryAndTopology(shader,mesh);
    }

    virtual void render(Mesh& mesh, Window& window){
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        shader->use();

        toGPUniformSender.sendUniforms(shader);

        GLuint numIndicies = mesh.getNumberIndicies();

        glPatchParameteri(GL_PATCH_VERTICES, 3);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawElements(GL_PATCHES, numIndicies, GL_UNSIGNED_SHORT, nullptr);

        shader->unuse();
        window.swapBuffers();
    };

    OnChangeListener<std::pair<int, int>> &getOnWindowResizeProjectionUpdater() {
        return onWindowResizeProjectionUpdater;
    }
};


#endif //GAMEENGINE_RENDERER_H
