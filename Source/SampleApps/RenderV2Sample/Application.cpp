//
// Created by dominik on 17.03.19.
//

#include "Source/GraphicsLayer/OpenGLinitializer.h"
#include "Application.h"

#include "Source/GraphicsLayer/RenderSystemV2/Buffers/VertexArrayObject.h"
#include "Source/GraphicsLayer/RenderSystemV2/Buffers/AttributeBuffer.h"
#include "Source/GraphicsLayer/RenderSystemV2/Buffers/ElementArrayBuffer.h"
#include "Source/GraphicsLayer/RenderSystemV2/Metadata/AttributeMetadata.h"
#include "Source/GraphicsLayer/RenderSystemV2/Buffers/UniformBuffer.h"

#include "Source/DataLayer/DataTypes/Geometry/MeshGenerator.h"


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

    entitySystem.addMesh("Triangle", MeshGenerator::generateTriangeMesh());
    entitySystem.addEntity("T1", entitySystem.entityFactory.make("Triangle", glm::vec3(0.f,0.f,0.f)));
    entitySystem.addEntity("T2", entitySystem.entityFactory.make("Triangle", glm::vec3(1.f,0.f,0.f)));
    entitySystem.addEntity("T3", entitySystem.entityFactory.make("Triangle", glm::vec3(-1.f,0.f,0.f)));
    entitySystem.addEntity("T4", entitySystem.entityFactory.make("Triangle", glm::vec3(2.f,2.f,0.f)));
}

void Application::main() {
    shader->use();

    VertexArrayObject vao;
    vao.bind();

    const std::shared_ptr<Mesh> mesh = entitySystem.getMesh("Triangle");
    const std::vector<glm::mat4>* models = entitySystem.getAllModelsForMesh("Triangle");

    AttributeBuffer attributeBuffer = AttributeBufferFactory()
            .insert( AttributeMetadata(0, 3, GL_FLOAT, 0, 2*sizeof(glm::vec3)))
            .insert( AttributeMetadata(1, 3, GL_FLOAT, sizeof(glm::vec3), 2*sizeof(glm::vec3)) )
            .make();

    AttributeBuffer modelBuffer = AttributeBufferFactory()
            //insert a Matrix, which is a set of 4 vec4's
            .insert( AttributeMetadata( 2, 4, GL_FLOAT, 0,
                                        4 * sizeof(glm::vec4), 1) )
            .insert( AttributeMetadata( 3, 4, GL_FLOAT, sizeof(glm::vec4),
                                        4 * sizeof(glm::vec4), 1) )
            .insert( AttributeMetadata( 4, 4, GL_FLOAT, 2 * sizeof(glm::vec4),
                                        4 * sizeof(glm::vec4), 1) )
            .insert( AttributeMetadata( 5, 4, GL_FLOAT, 3 * sizeof(glm::vec4),
                                        4 * sizeof(glm::vec4), 1) )
            .make();

    ElementArrayBuffer elementArrayBuffer;

    UniformBuffer uniformBuffer = UniformBufferFactory()
            .setBinding(0)
            .insert( UniformMetadata( &View, GL_FLOAT_MAT4 ) )
            .insert( UniformMetadata( &Projection, GL_FLOAT_MAT4 ) )
            .make();

    attributeBuffer.bind();
    attributeBuffer.enableAllAttribsAndSpecifyTheirOffsetsIfVaoBinded();
    attributeBuffer.sendBufferToGPUifVaoBinded(mesh->getVerticies());

    modelBuffer.bind();
    modelBuffer.enableAllAttribsAndSpecifyTheirOffsetsIfVaoBinded();
    modelBuffer.sendBufferToGPUifVaoBinded(*models);

    elementArrayBuffer.bind();
    elementArrayBuffer.sendIfVaoEnabled(mesh->getIndicies());

    uniformBuffer.bind();
    uniformBuffer.bakeData();
    uniformBuffer.sendBufferToGPU();

    while(window->isRunning()){
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

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
