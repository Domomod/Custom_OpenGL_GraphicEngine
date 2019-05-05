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

    try {
        basicShader->loadFromFile(Shader::VERTEX, "../Shaders/basic.vs");
        basicShader->loadFromFile(Shader::FRAGMENT, "../Shaders/basic.fs");
        basicShader->createAndLinkProgram();

        texturedShader->loadFromFile(Shader::VERTEX, "../Shaders/BasicTextured/basic.vs");
        texturedShader->loadFromFile(Shader::FRAGMENT, "../Shaders/BasicTextured/basic.fs");
        texturedShader->createAndLinkProgram();

        animationShader->loadFromFile(Shader::VERTEX, "../Shaders/AnimationShader/animated.vs");
        animationShader->loadFromFile(Shader::FRAGMENT, "../Shaders/AnimationShader/animated.fs");
        animationShader->createAndLinkProgram();
    } catch( MyException& e) {
        std::cerr << e.getType() << ":\n" << e.getMessage();
        throw e;
    }

    View = glm::lookAt(glm::vec3(0.f,0.f,3.f), glm::vec3(0.f,0.f,0.f), glm::vec3(0.f,1.f,0.f));
    Projection = glm::perspective(glm::radians(45.f),8.f/4.f, 1.f, 150.f);

    windowResizeListener.setReactionFuncPtr([&](std::pair<int,int> size){
        Projection =  glm::perspective(glm::radians(45.f),(float)size.first / (float)size.second, 1.f, 30.f);
    });

    window->getResizeNotifierPtr()->addListener(&windowResizeListener);

    try {
        entitySystem.addModel("Mech", ModelLoader::loadModel("Models/WornMetalKnight/Knight.obj"));
        entitySystem.addEntity("M1", entitySystem.entityFactory.make("Mech", glm::vec3(0, 0, -10), 3.0f));

        entitySystem.addModel("Cowboy", ModelLoader::loadModel("Models/Cowboy/cowboy.dae"));
        entitySystem.addEntity("C1", entitySystem.entityFactory.make("Cowboy", glm::vec3(0, -5, -20), glm::vec3(-90.f, 0.f, 0.f)) );


    } catch (MeshLoadingException& e){
        std::cerr << e.getMessage();
        exit(1);
    }
}

void Application::main() {

    const auto cowboy = entitySystem.getEntity("C1");
    const auto cowboyModel = cowboy->getModel();

    const auto mech = entitySystem.getEntity("M1");
    const auto mechModel = mech->getModel();


    VertexArrayObject vao;
    vao.bind();

    AttributeBuffer posBuffer = AttributeBufferFactory()
            .insert( AttributeMetadata(0, 4, GL_FLOAT, 0, sizeof(glm::vec4)))
            .make();

    AttributeBuffer texCoordBuffer = AttributeBufferFactory()
            .insert( AttributeMetadata(1, 2, GL_FLOAT, 0, sizeof(glm::vec2)))
            .make();

    AttributeBuffer colorBuffer = AttributeBufferFactory()
            .insert( AttributeMetadata(1, 4, GL_FLOAT, 0, sizeof(glm::vec4)))
            .make();

    AttributeBuffer boneIdsBuffer = AttributeBufferFactory()
            .insert( AttributeMetadata(2, 4, GL_FLOAT, 0, sizeof(glm::vec4)))
            .make();

    AttributeBuffer boneWeightsBuffer = AttributeBufferFactory()
            .insert( AttributeMetadata(3, 4, GL_FLOAT, 0, sizeof(glm::vec4)))
            .make();

    ElementArrayBuffer elementArrayBuffer;

    UniformBuffer animatedUniformBuffer = UniformBufferFactory()
            .setBinding(0)
            .insert( UniformMetadata( &ModelViewProjection, GL_FLOAT_MAT4 ) )
            .insert( UniformMetadata( cowboyModel->animator->getCurrentPoseTransformation(), GL_FLOAT_MAT4, SkeletalSystem::MAX_BONES) )
            .make();

    UniformBuffer basicShaderBuffer = UniformBufferFactory()
            .setBinding(0)
            .insert( UniformMetadata( &ModelViewProjection, GL_FLOAT_MAT4 ) )
            .make();

    std::shared_ptr<Texture> cowboyTexture = TextureLoader::loadTexture("Textures/cowboy.png");

    /*TODO: move animator to entity because multiple entities schould have independent animations*/
    cowboyModel->animator->setCurrentAnimation(cowboyModel->skeletalAnimations[0]);

    glEnable(GL_DEPTH_TEST);
    glPatchParameteri(GL_PATCH_VERTICES, 3);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    while(window->isRunning()){
        time = static_cast<float>(glfwGetTime());
        View = freeCamera->calculateViewMatrix();

        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        texturedShader->use();
        ModelViewProjection = Projection * View * mech->getModelSpaceMatrix();
        for( auto& mesh : mechModel->meshes) {

            mesh->normalMap->bind();

            posBuffer.bind();
            posBuffer.sendBufferToGPUifVaoBinded(mesh->positions);

            texCoordBuffer.bind();
            texCoordBuffer.sendBufferToGPUifVaoBinded(mesh->uv);

            elementArrayBuffer.bind();
            elementArrayBuffer.sendIfVaoEnabled(mesh->indicies);

            animatedUniformBuffer.bind();
            animatedUniformBuffer.bakeData();
            animatedUniformBuffer.sendBufferToGPU();

            glDrawElements(GL_TRIANGLES, mesh->indicies.size(), GL_UNSIGNED_SHORT, nullptr);
        }
        texturedShader->unuse();



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
