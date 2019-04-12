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

#include "Source/DataLayer/DataTypes/Assets/MeshGenerator.h"
#include "Source/DataLayer/DataTypes/Assets/ModelLoader.h"
#include "Source/DataLayer/DataTypes/Assets/Model.h"
#include "Source/DataLayer/DataTypes/Assets/Textures/Texture.h"
#include "Source/DataLayer/DataTypes/Assets/Textures/TextureLoader.h"

Application::Application() {
    openGlInitalizer = std::make_shared<OpenGlInitalizer>();
    openGlInitalizer->initGLFW();
    window = std::make_shared<Window>();
    window->makeCurrent();
    openGlInitalizer->initOpenGL();

    freeCamera = std::make_shared<FreeCamera>();


    windowInputSystem = std::make_shared<WindowInputSystem>();
    windowInputSystem->connectToWindow(*window);
    windowInputSystem->connectToKeyboardStateListener(freeCamera->getKeyboardStateListener());
    windowInputSystem->connectToMouseMovedListener(freeCamera->getMouseMovementListener());

    shader = std::make_shared<Shader>();
    waterShader = std::make_shared<Shader>();

    try {
        shader->loadFromFile(Shader::VERTEX, "../Shaders/BasicTextured/basic_instanced.vs");
        shader->loadFromFile(Shader::FRAGMENT, "../Shaders/BasicTextured/basic.fs");
        shader->createAndLinkProgram();

        waterShader->loadFromFile(Shader::VERTEX, "../Shaders/WaterShader/water_instanced.vs");
        waterShader->loadFromFile(Shader::TESSELATION_CONTROL, "../Shaders/WaterShader/water.tcs");
        waterShader->loadFromFile(Shader::TESSELATION_EVALUATION, "../Shaders/WaterShader/water.tes");
        waterShader->loadFromFile(Shader::FRAGMENT, "../Shaders/WaterShader/water.fs");
        waterShader->createAndLinkProgram();
    } catch( MyException& e) {
        std::cerr << e.getType() << ":\n" << e.getMessage();
        throw e;
        //TODO: ask for proper path if sth goes wrong
    }

    View = glm::lookAt(glm::vec3(0.f,0.f,3.f), glm::vec3(0.f,0.f,0.f), glm::vec3(0.f,1.f,0.f));
    Projection = glm::perspective(glm::radians(45.f),8.f/4.f, 1.f, 30.f);

    windowResizeListener.setReactionFuncPtr([&](std::pair<int,int> size){
       Projection =  glm::perspective(glm::radians(45.f),(float)size.first / (float)size.second, 1.f, 30.f);
    });

    window->getResizeNotifierPtr()->addListener(&windowResizeListener);

    entitySystem.addModel("Triangle",   ModelFactory()
                                          .addMesh( MeshGenerator::generateTriangeMesh() )
                                          .make()
                                          );
    entitySystem.addEntity("T1", entitySystem.entityFactory.make("Triangle", glm::vec3(0.f,0.f,0.f)));
    entitySystem.addEntity("T2", entitySystem.entityFactory.make("Triangle", glm::vec3(1.f,0.f,0.f)));
    entitySystem.addEntity("T3", entitySystem.entityFactory.make("Triangle", glm::vec3(-1.f,0.f,0.f)));
    entitySystem.addEntity("T4", entitySystem.entityFactory.make("Triangle", glm::vec3(2.f,2.f,0.f)));

    entitySystem.addModel("Quad",       ModelFactory()
                                          .addMesh( MeshGenerator::generateSimpleRectangleMesh(10, -1, 10) )
                                          .make()
                                          );

    entitySystem.addEntity("Q1", entitySystem.entityFactory.make("Quad", glm::vec3(-10, -1.7, -20)));
    entitySystem.addEntity("Q2", entitySystem.entityFactory.make("Quad", glm::vec3(10 , -1.7, -20)));
    entitySystem.addEntity("Q3", entitySystem.entityFactory.make("Quad", glm::vec3(-10, -1.7, 0  )));
    entitySystem.addEntity("Q4", entitySystem.entityFactory.make("Quad", glm::vec3(10 , -1.7, 0  )));

    entitySystem.addModel("Barrel", ModelLoader::loadModel("Meshes/barrel.obj"));
    entitySystem.addEntity("B1", entitySystem.entityFactory.make("Barrel", glm::vec3(0,0,-10)));
}

void Application::main() {
    VertexArrayObject vao;
    vao.bind();

    AttributeBuffer posBuffer = AttributeBufferFactory()
            .insert( AttributeMetadata(0, 4, GL_FLOAT, 0, sizeof(glm::vec4)))
            .make();

    AttributeBuffer colBuffer = AttributeBufferFactory()
            .insert( AttributeMetadata(1, 4, GL_FLOAT, 0, sizeof(glm::vec4)))
            .make();

    AttributeBuffer texCoordBuffer = AttributeBufferFactory()
            .insert( AttributeMetadata(1, 2, GL_FLOAT, 0, sizeof(glm::vec2)))
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

    UniformBuffer instancedUniformBuffer = UniformBufferFactory()
            .setBinding(0)
            .insert( UniformMetadata( &View,       GL_FLOAT_MAT4 ) )
            .insert( UniformMetadata( &Projection, GL_FLOAT_MAT4 ) )
            .make();

    UniformBuffer waterUniformBuffer = UniformBufferFactory()
            .setBinding(1)
            .insert( UniformMetadata( &center,     GL_FLOAT_VEC4 ) )
            .insert( UniformMetadata( &time,       GL_FLOAT      ) )
            .insert( UniformMetadata( &amplitude,  GL_FLOAT      ) )
            .insert( UniformMetadata( &frequency,  GL_FLOAT      ) )
            .make();

    const std::shared_ptr<Model> triangle = entitySystem.getModel("Triangle");
    const std::vector<glm::mat4>* triangle_models = entitySystem.getAllFromModelSpaceMatricesForModel("Triangle");

    const std::shared_ptr<Model> quad = entitySystem.getModel("Quad");
    const std::vector<glm::mat4>* quad_models = entitySystem.getAllFromModelSpaceMatricesForModel("Quad");

    const std::shared_ptr<Model> barrel = entitySystem.getModel("Barrel");
    const std::vector<glm::mat4>* barrel_models = entitySystem.getAllFromModelSpaceMatricesForModel("Barrel");


    std::shared_ptr<Texture> barrelTexture = TextureLoader::loadTexture("Textures/barrel.png");

    barrelTexture->bind();

    glEnable(GL_DEPTH_TEST);
    glPatchParameteri(GL_PATCH_VERTICES, 3);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    while(window->isRunning()){
        time = static_cast<float>(glfwGetTime());
        View = freeCamera->calculateViewMatrix();

        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        //DRAW TRIANGLES
        waterShader->use();

        posBuffer.bind();
        posBuffer.sendBufferToGPUifVaoBinded( quad->mesh->positions );

        colBuffer.bind();
        colBuffer.sendBufferToGPUifVaoBinded( quad->mesh->colors );

        modelBuffer.bind();
        modelBuffer.sendBufferToGPUifVaoBinded( *quad_models );

        elementArrayBuffer.bind();
        elementArrayBuffer.sendIfVaoEnabled( quad->mesh->indicies );

        instancedUniformBuffer.bind();
        instancedUniformBuffer.bakeData();
        instancedUniformBuffer.sendBufferToGPU();

        waterUniformBuffer.bind();
        waterUniformBuffer.bakeData();
        waterUniformBuffer.sendBufferToGPU();

        glDrawElementsInstanced(GL_PATCHES, quad->mesh->indicies.size(), GL_UNSIGNED_SHORT, nullptr, quad_models->size());

        shader->use();

        posBuffer.bind();
        posBuffer.sendBufferToGPUifVaoBinded( barrel->mesh->positions );

        texCoordBuffer.bind();
        texCoordBuffer.sendBufferToGPUifVaoBinded( barrel->mesh->uv );

        modelBuffer.bind();
        modelBuffer.sendBufferToGPUifVaoBinded( *barrel_models );

        elementArrayBuffer.bind();
        elementArrayBuffer.sendIfVaoEnabled( barrel->mesh->indicies );

        glDrawElementsInstanced(GL_TRIANGLES, barrel->mesh->indicies.size(), GL_UNSIGNED_SHORT, nullptr, barrel_models->size());

        window->swapBuffers();
        glfwPollEvents();
        windowInputSystem->keyboardStateNotify();
    }

    waterShader->unuse();

    elementArrayBuffer.unbind();
    posBuffer.unbind();
    vao.unbind();
}

Application::~Application() {

}
