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

    basicShader = std::make_shared<Shader>();
    texturedShader = std::make_shared<Shader>();
    animationShader = std::make_shared<Shader>();
    pbrShader =  std::make_shared<Shader>();

    try {
        basicShader->loadFromFile(Shader::VERTEX, "../Shaders/basic.vs");
        basicShader->createAndLinkProgram();

        texturedShader->loadFromFile(Shader::VERTEX, "../Shaders/BasicTextured/basic.vs");
        texturedShader->loadFromFile(Shader::FRAGMENT, "../Shaders/BasicTextured/basic.fs");
        texturedShader->createAndLinkProgram();

        animationShader->loadFromFile(Shader::VERTEX, "../Shaders/AnimationShader/animated.vs");
        animationShader->loadFromFile(Shader::FRAGMENT, "../Shaders/AnimationShader/animated.fs");
        animationShader->createAndLinkProgram();

        pbrShader->loadFromFile(Shader::VERTEX, "../Shaders/FORWARD_PBR_RM_TANGENT_SPACE/vertex.glsl");
        pbrShader->loadFromFile(Shader::FRAGMENT, "../Shaders/FORWARD_PBR_RM_TANGENT_SPACE/fragment.glsl");
        pbrShader->createAndLinkProgram();
    } catch( MyException& e) {
        std::cerr << e.getType() << ":\n" << e.getMessage();
        exit(1);
    }

    View = glm::lookAt(glm::vec3(0.f,0.f,3.f), glm::vec3(0.f,0.f,0.f), glm::vec3(0.f,1.f,0.f));
    Projection = glm::perspective(glm::radians(45.f),8.f/4.f, 1.f, 150.f);

    windowResizeListener.setReactionFuncPtr([&](std::pair<int,int> size){
        Projection =  glm::perspective(glm::radians(45.f),(float)size.first / (float)size.second, 1.f, 30.f);
    });

    window->getResizeNotifierPtr()->addListener(&windowResizeListener);

    try {
        entitySystem.addModel("Mech", ModelLoader::loadModel("Models/CleanMetalKnight/Knight.obj"));
        entitySystem.addEntity("M1", entitySystem.entityFactory.make("Mech", glm::vec3(-1.5, 0, 0), 3.0f));

        entitySystem.addModel("Cowboy", ModelLoader::loadModel("Models/WornMetalKnight/Knight.obj"));
        entitySystem.addEntity("C1", entitySystem.entityFactory.make("Cowboy", glm::vec3(1.5, 0, 0), 3.0f));


    } catch (MeshLoadingException& e){
        std::cerr << e.getMessage();
        exit(1);
    }
}

void Application::main() {

    std::vector<std::shared_ptr<Entity>> entities;
    entities.push_back(entitySystem.getEntity("M1"));
    entities.push_back(entitySystem.getEntity("C1"));


    VertexArrayObject vao;
    vao.bind();

    AttributeBuffer posBuffer = AttributeBufferFactory()
            .insert( AttributeMetadata(0, 3, GL_FLOAT, 0, sizeof(glm::vec3)))
            .make();

    AttributeBuffer texCoordBuffer = AttributeBufferFactory()
            .insert( AttributeMetadata(1, 2, GL_FLOAT, 0, sizeof(glm::vec2)))
            .make();

    AttributeBuffer normalBuffer = AttributeBufferFactory()
            .insert( AttributeMetadata(2, 3, GL_FLOAT, 0, sizeof(glm::vec3)))
            .make();

    AttributeBuffer tangentBuffer = AttributeBufferFactory()
            .insert( AttributeMetadata(3, 3, GL_FLOAT, 0, sizeof(glm::vec3)))
            .make();

    ElementArrayBuffer elementArrayBuffer;

    auto ViewerPos = glm::vec3(0.f);

    UniformBuffer basicShaderBuffer = UniformBufferFactory()
            .setBinding(0)
            .insert( UniformMetadata( &ModelViewProjection, GL_FLOAT_MAT4 ) )
            .insert( UniformMetadata( &Model, GL_FLOAT_MAT4 ) )
            .insert( UniformMetadata( &ViewerPos, GL_FLOAT_MAT4 ) )
            .make();

    std::shared_ptr<Texture> cowboyTexture = TextureLoader::loadTexture("Textures/cowboy.png");

    /*TODO: move animator to entity because multiple entities schould have independent animations*/

    glEnable(GL_DEPTH_TEST);
    glPatchParameteri(GL_PATCH_VERTICES, 3);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    while(window->isRunning()){
        time = static_cast<float>(glfwGetTime());
        View = freeCamera->calculateViewMatrix();
        ViewerPos = freeCamera->getPosition();

        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        pbrShader->use();
        for(auto & entity : entities) {
            ModelViewProjection = Projection * View * entity->getModelSpaceMatrix();
            Model = entity->getModelSpaceMatrix();
            for (auto &mesh : entity->getModel()->meshes) {

                mesh->albedoMap->bind(0);
                mesh->aoMap->bind(1);
                mesh->metallnessMap->bind(2);
                mesh->roughnessMap->bind(3);
                mesh->normalMap->bind(4);

                posBuffer.bind();
                posBuffer.sendBufferToGPUifVaoBinded(mesh->positions);

                texCoordBuffer.bind();
                texCoordBuffer.sendBufferToGPUifVaoBinded(mesh->uv);

                normalBuffer.bind();
                normalBuffer.sendBufferToGPUifVaoBinded(mesh->normals);

                tangentBuffer.bind();
                tangentBuffer.sendBufferToGPUifVaoBinded(mesh->tangents);

                elementArrayBuffer.bind();
                elementArrayBuffer.sendIfVaoEnabled(mesh->indicies);

                basicShaderBuffer.bind();
                basicShaderBuffer.bakeData();
                basicShaderBuffer.sendBufferToGPU();

                glDrawElements(GL_TRIANGLES, mesh->indicies.size(), GL_UNSIGNED_SHORT, nullptr);
            }
        }
        pbrShader->unuse();

        ModelViewProjection = Projection * View;

        texCoordBuffer.unbind();
        normalBuffer.unbind();
        tangentBuffer.unbind();
        elementArrayBuffer.unbind();

        basicShader->use();
        std::vector<glm::vec3> lightPos = {glm::vec3(0.3,5.0,0.6)};
        posBuffer.bind();
        posBuffer.sendBufferToGPUifVaoBinded(lightPos);

        basicShaderBuffer.bakeData();
        basicShaderBuffer.sendBufferToGPU();

        glPointSize(20);
        glDrawArrays(GL_POINTS, 0, lightPos.size());
        basicShader->unuse();


        window->swapBuffers();
        glfwPollEvents();
        windowInputSystem->keyboardStateNotify();
    }

    elementArrayBuffer.unbind();
    posBuffer.unbind();
    vao.unbind();
}

Application::~Application() {

}
