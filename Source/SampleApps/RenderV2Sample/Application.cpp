//
// Created by dominik on 17.03.19.
//

#include <Source/OpenGL/OpenGLinitializer.h>
#include "Application.h"

#include "Source/OpenGL/RenderSystemV2/Buffers/VertexArrayObject.h"
#include "Source/OpenGL/RenderSystemV2/Buffers/AttributeBuffer.h"
#include "Source/OpenGL/RenderSystemV2/Buffers/ElementArrayBuffer.h"
#include "Source/OpenGL/RenderSystemV2/Metadata/AttributeMetadata.h"
#include "Source/OpenGL/RenderSystemV2/Buffers/UniformBuffer.h"

Application::Application() {
    openGlInitalizer = std::make_shared<OpenGlInitalizer>();
    openGlInitalizer->initGLFW();
    window = std::make_shared<Window>();
    window->makeCurrent();
    openGlInitalizer->initOpenGL();
    shader = std::make_shared<Shader>();

    try {
        shader->loadFromFile(Shader::VERTEX, "../Shaders/basic_instanced.vs");
        shader->loadFromFile(Shader::FRAGMENT, "../Shaders/basic.fs");
        shader->createAndLinkProgram();
        shader->use();
        shader->unuse();
    } catch( MyException& e) {
        std::cerr << e.getType() << ":\n" << e.getMessage();
        throw e;
        //TODO: ask for proper path if sth goes wrong
    }

    Model = glm::mat4(1);
    View = glm::lookAt(glm::vec3(0.f,0.f,3.f), glm::vec3(0.f,0.f,0.f), glm::vec3(0.f,1.f,0.f));
    Projection = glm::perspective(glm::radians(45.f),8.f/4.f, 1.f, 30.f);
    ModelViewProjection = Projection * View * Model;

    windowResizeListener.setReactionFuncPtr([&](std::pair<int,int> size){
       Projection =  glm::perspective(glm::radians(45.f),(float)size.first / (float)size.second, 1.f, 30.f);
        ModelViewProjection = Projection * View * Model;
    });

    window->getResizeNotifierPtr()->addListener(&windowResizeListener);

}

void Application::main() {
    shader->use();

    VertexArrayObject vao;
    vao.bind();

    AttributeBuffer attributeBuffer = AttributeBufferFactory()
            .insert( AttributeMetadata(0, 3, GL_FLOAT, 0, sizeof(glm::vec3)))
            .make();

    attributeBuffer.bind();
    attributeBuffer.enableAllAttribsAndSpecifyTheirOffsetsIfVaoBinded();
    attributeBuffer.sendBufferToGPUifVaoBinded(instancedModels.positions);

    AttributeBuffer modelBuffer = AttributeBufferFactory()
                    //insert a Matrix, which is a set of 4 vec4's
            .insert( AttributeMetadata( 1, 4, GL_FLOAT, 0,
                                        4 * sizeof(glm::vec4), 1) )
            .insert( AttributeMetadata( 2, 4, GL_FLOAT, sizeof(glm::vec4),
                                        4 * sizeof(glm::vec4), 1) )
            .insert( AttributeMetadata( 3, 4, GL_FLOAT, 2 * sizeof(glm::vec4),
                                        4 * sizeof(glm::vec4), 1) )
            .insert( AttributeMetadata( 4, 4, GL_FLOAT, 3 * sizeof(glm::vec4),
                                        4 * sizeof(glm::vec4), 1) )
            .make();

    modelBuffer.bind();
    modelBuffer.enableAllAttribsAndSpecifyTheirOffsetsIfVaoBinded();
    modelBuffer.sendBufferToGPUifVaoBinded(instancedModels.ModelMatrixes);

    ElementArrayBuffer elementArrayBuffer;

    elementArrayBuffer.bind();
    elementArrayBuffer.sendIfVaoEnabled(indicies);

    UniformBuffer uniformBuffer = UniformBufferFactory()
            .setBinding(0)
            .insert( UniformMetadata( &color, GL_FLOAT_VEC4 ) )
            .insert( UniformMetadata( &View, GL_FLOAT_MAT4 ) )
            .insert( UniformMetadata( &Projection, GL_FLOAT_MAT4 ) )
            .make();

    uniformBuffer.bind();
    uniformBuffer.bakeData();
    uniformBuffer.sendBufferToGPU();

    while(window->isRunning()){
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        color = glm::vec4( sin(glfwGetTime()), cos(glfwGetTime()), sin(glfwGetTime()) * cos(glfwGetTime()) ,1);
        uniformBuffer.bakeData();
        uniformBuffer.sendBufferToGPU();
        glDrawElementsInstanced(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, nullptr, 4);
        glfwPollEvents();
        window->swapBuffers();
    }
    shader->unuse();
    elementArrayBuffer.unbind();
    attributeBuffer.unbind();
    vao.unbind();
}

Application::~Application() {

}
