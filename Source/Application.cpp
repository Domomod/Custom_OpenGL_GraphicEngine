//
// Created by dominik on 17.03.19.
//

#include "Platform/OpenGL/OpenGLinitializer.h"
#include "Application.h"

#include "Platform/OpenGL/Buffers/VertexArrayObject.h"
#include "Platform/OpenGL/Buffers/AttributeBuffer.h"
#include "Platform/OpenGL/Buffers/ElementArrayBuffer.h"
#include "Platform/OpenGL/Buffers/Metadata/AttributeMetadata.h"
#include "Platform/OpenGL/Buffers/UniformBuffer.h"

#include "Assets/Mesh/MeshGenerator.h"
#include "Assets/Model.h"
#include "Assets/Textures/Texture.h"
#include "Assets/Textures/TextureLoader.h"
#include "Assets/Textures/MaterialsLoader.h"

Application::Application()
{

    loadShaders();

    windowInputSystem.connectToWindow(window);
    windowInputSystem.connectToKeyboardStateListener(freeCamera.getKeyboardStateListener());
    windowInputSystem.connectToKeyPressedListener(keyActionListener);
    windowInputSystem.connectToMouseMovedListener(freeCamera.getMouseMovementListener());

    /*Set application reaction to keyboard state notify*/
    keyActionListener.setReactionFuncPtr(
            [&](KeyInfo keyInfo)
            {
                if (keyInfo.key == GLFW_KEY_R && keyInfo.action == GLFW_PRESS)
                { this->loadShaders(); }

                if (keyInfo.key == GLFW_KEY_T && keyInfo.action == GLFW_PRESS)
                {
                    if (this->scene == &this->entities)
                    {
                        this->scene = &(this->entities2);
                    }
                    else
                    {
                        this->scene = &this->entities;
                    }
                }
            }
    );

    View = glm::lookAt(glm::vec3(0.f, 0.f, 3.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));
    Projection = glm::perspective(glm::radians(45.f), 8.f / 4.f, 1.f, 150.f);

    windowResizeListener.setReactionFuncPtr([&](std::pair<int, int> size)
                                            {
                                                Projection = glm::perspective(glm::radians(45.f),
                                                                              (float) size.first / (float) size.second,
                                                                              1.f, 30.f);
                                            });

    window.getResizeNotifierPtr()->addListener(&windowResizeListener);

    try
    {
        /*TODO: add material alliasses to only load one model with many materials to choose*/
        entitySystem.addModel("Mech", modelLoader.useEmbededMaterials()
                .loadModel("Models/CleanMetalKnight/Knight.obj"));
        entitySystem.addEntity("M1", entitySystem.entityFactory.make("Mech", glm::vec3(-1.5, -0.5, -1), 3.0f));

        entitySystem.addModel("Cowboy", modelLoader.useEmbededMaterials()
                                                   .loadModel("Models/WornMetalKnight/Knight.obj"));
        entitySystem.addEntity("C1", entitySystem.entityFactory.make("Cowboy", glm::vec3(1.5, -0.5, -1), 3.0f));

        entitySystem.addModel("Sphere1", modelLoader.useSpecificMaterial("Materials/grimy_metal/grimy_metal.Material")
                                                    .loadModel("Meshes/low_poly_sphere.obj"));
        entitySystem.addEntity("S1", entitySystem.entityFactory.make("Sphere1", glm::vec3( 3.0, -0.5, 2.0)));

        entitySystem.addModel("Sphere3", modelLoader.useSpecificMaterial("Materials/metal_slpotchy/metal_slpotchy.Material")
                                                    .loadModel("Meshes/low_poly_sphere.obj"));
        entitySystem.addEntity("S3", entitySystem.entityFactory.make("Sphere3", glm::vec3( 0.0, -0.5, 2.0)));


        entitySystem.addModel("Sphere5", modelLoader.useSpecificMaterial("Materials/scuffed_plastic/scuffed_plastic_pink.Material")
                                                    .loadModel("Meshes/low_poly_sphere.obj"));
        entitySystem.addEntity("S5", entitySystem.entityFactory.make("Sphere5", glm::vec3(-3.0, -0.5, 2.0)));

        entitySystem.addModel("Plane", modelLoader.useSpecificMaterial("Materials/mirror/mirror.Material")
                .loadModel("Meshes/verticalPlane.obj"));
        entitySystem.addEntity("P1", entitySystem.entityFactory.make("Plane", glm::vec3(-3.0, -0.5, -2.0)));

    }
    catch (MeshLoadingException &e)
    {
        std::cerr << e.getMessage();
        exit(1);
    }
}

void Application::main()
{
    auto waterfallSkybox = textureLoader.calculateCubeMapFromEquirectangularTexture(
            textureLoader.loadTexture2D_F("Textures/equirectangular/Frozen_Waterfall/Waterfall_Skybox.hdr") );

    auto waterfallIrradiance = textureLoader.calculateCubeMapFromEquirectangularTexture(
            textureLoader.loadTexture2D_F("Textures/equirectangular/Frozen_Waterfall/Waterfall_Irradiance.hdr") );

    auto waterfallRadiance = textureLoader.calculatePrefilteredEnviromentMap(waterfallSkybox);


    auto brdfLUT = textureLoader.loadTexture2D("Textures/ibl_brdf_lut.png");

    auto skyboxMesh = MeshGenerator::generateSkyBox();

    entities.push_back(entitySystem.getEntity("M1"));
    entities.push_back(entitySystem.getEntity("C1"));

    entities2.push_back(entitySystem.getEntity("S1"));
    entities2.push_back(entitySystem.getEntity("S3"));
    entities2.push_back(entitySystem.getEntity("S5"));
    entities2.push_back(entitySystem.getEntity("P1"));

    scene = &entities;


    VertexArrayObject vao;
    vao.bind();

    AttributeBuffer posBuffer = AttributeBufferFactory()
            .insert(AttributeMetadata(0, 3, GL_FLOAT, 0, sizeof(glm::vec3)))
            .make();

    auto ViewerPos = glm::vec3(0.f);
    float MAX_REFLECTION_LOD = waterfallRadiance->getMipmapCount() - 1;

    UniformBuffer basicShaderBuffer = UniformBufferFactory()
            .setBinding(0)
            .insert(UniformMetadata(&ModelViewProjection, GL_FLOAT_MAT4))
            .insert(UniformMetadata(&Model, GL_FLOAT_MAT4))
            .insert(UniformMetadata(&ViewerPos, GL_FLOAT_MAT4))
            .insert(UniformMetadata(&MAX_REFLECTION_LOD, GL_FLOAT))
            .make();

    UniformBuffer skyShaderBuffer = UniformBufferFactory()
            .setBinding(0)
            .insert(UniformMetadata(&Projection, GL_FLOAT_MAT4))
            .insert(UniformMetadata(&View, GL_FLOAT_MAT4))
            .make();

    /*TODO: move animator to entity because multiple entities schould have independent animations*/

    glEnable(GL_DEPTH_TEST);
    glPatchParameteri(GL_PATCH_VERTICES, 3);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDepthFunc(GL_LEQUAL);

    brdfLUT->bind(5);
    waterfallRadiance->bind(6);
    waterfallIrradiance->bind(7);
    waterfallRadiance->bind(8);

    while (window.isRunning())
    {
        if (shadersCompiled)
        {
            time = static_cast<float>(glfwGetTime());
            View = freeCamera.calculateViewMatrix();
            ViewerPos = freeCamera.getPosition();

            glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

            basicShaderBuffer.bind();



            pbrShader->use();
            for (auto &entity : *scene)
            {
                ModelViewProjection = Projection * View * entity->getModelSpaceMatrix();
                Model = entity->getModelSpaceMatrix();


                basicShaderBuffer.bakeData();
                basicShaderBuffer.sendBufferToGPU();


                /* DRAW KNIGHTS
                 * */
                for (auto &mesh : entity->getModel()->meshes)
                {
                    mesh->bindTexturesPBR();
                    mesh->bindVao();
                    glDrawElements(GL_TRIANGLES, mesh->getIndiciesCount(), GL_UNSIGNED_SHORT, nullptr);
                }
            }
            pbrShader->unuse();

            ModelViewProjection = Projection * View;

            /* DRAW LIGHTS
             * */
            basicShader->use();
            std::vector<glm::vec3> lightPos = {glm::vec3(0.3, 5.0, 0.6)};
            vao.bind();
            posBuffer.bind();
            posBuffer.sendBufferToGPUifVaoBinded(lightPos);

            basicShaderBuffer.bakeData();
            basicShaderBuffer.sendBufferToGPU();

            glPointSize(20);
            glDrawArrays(GL_POINTS, 0, lightPos.size());
            vao.unbind();
            basicShader->unuse();

            /* DRAW SKYBOX
             * */
            skyBoxShader->use();

            skyboxMesh->bindVao();

            skyShaderBuffer.bind();
            skyShaderBuffer.bakeData();
            skyShaderBuffer.sendBufferToGPU();

            glDrawElements(GL_TRIANGLES, skyboxMesh->getIndiciesCount(), GL_UNSIGNED_SHORT, nullptr);

            skyBoxShader->unuse();
            window.swapBuffers();
        }
        windowInputSystem.pollEvents();
    }
}

Application::~Application()
{

}

void Application::loadShaders()
{
    freeResources();
    try
    {
        basicShader->loadFromFile(Shader::VERTEX, "../Shaders/basic.vs");
        basicShader->createAndLinkProgram();

        texturedShader->loadFromFile(Shader::VERTEX, "../Shaders/BasicTextured/basic.vs");
        texturedShader->loadFromFile(Shader::FRAGMENT, "../Shaders/BasicTextured/basic.fs");
        texturedShader->createAndLinkProgram();

        animationShader->loadFromFile(Shader::VERTEX, "../Shaders/AnimationShader/animated.vs");
        animationShader->loadFromFile(Shader::FRAGMENT, "../Shaders/AnimationShader/animated.fs");
        animationShader->createAndLinkProgram();

        pbrShader->loadFromFile(Shader::VERTEX, "../Shaders/FORWARD_PBR_IBL/vertex.glsl");
        pbrShader->loadFromFile(Shader::FRAGMENT, "../Shaders/FORWARD_PBR_IBL/fragment.glsl");
        pbrShader->createAndLinkProgram();

        skyBoxShader->loadFromFile(Shader::VERTEX, "../Shaders/CubeMaps/centered_cube_map_vertex.glsl");
        skyBoxShader->loadFromFile(Shader::FRAGMENT, "../Shaders/CubeMaps/SkyBox/fragment.glsl");
        skyBoxShader->createAndLinkProgram();

        skyBoxFromEquirectangularImageShader->loadFromFile(Shader::VERTEX,
                                                           "../Shaders/CubeMaps/centered_cube_map_vertex.glsl");
        skyBoxFromEquirectangularImageShader->loadFromFile(Shader::FRAGMENT,
                                                           "../Shaders/CubeMaps/EquirSkyBox/fragment.glsl");
        skyBoxFromEquirectangularImageShader->createAndLinkProgram();

        diffuseIrradianceMapShader->loadFromFile(Shader::VERTEX, "../Shaders/CubeMaps/centered_cube_map_vertex.glsl");
        diffuseIrradianceMapShader->loadFromFile(Shader::FRAGMENT,
                                                 "../Shaders/CubeMaps/DiffuseIrradianceMap/fragment.glsl");
        diffuseIrradianceMapShader->createAndLinkProgram();

        shadersCompiled = true;
    }
    catch (MyException &e)
    { /*TODO: add hierarchy for shader exceptions*/
        std::cerr << e.getType() << ":\n" << e.getMessage();
        shadersCompiled = false;
    }
}

void Application::freeResources()
{
    basicShader->deleteProgram();
    texturedShader->deleteProgram();
    animationShader->deleteProgram();
    pbrShader->deleteProgram();
    skyBoxShader->deleteProgram();
    skyBoxFromEquirectangularImageShader->deleteProgram();
    diffuseIrradianceMapShader->deleteProgram();
}