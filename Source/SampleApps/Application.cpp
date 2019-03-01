//
// Created by dominik on 09.02.19.
//

#include "Application.h"
#include "MeshGenerator.h"
#include "Mesh.h"

#include <glm/gtc/type_ptr.hpp>


Application::Application() : mainShader(new Shader()), mainWindow("Game Engine", 800, 600) {
    mesh = MeshGenerator::generateSymetricalRectanuglarMesh(2, 2, 3.f, 3.f, center.x, center.y,
                                                     center.z);

    Projection = glm::perspective(FOV, aspect, zNear, zFar);

    ModelViewProjection = Projection * ModelView;

    onWindowResizeProjectionUpdater.setReactionFuncPtr([&](std::pair<int,int> newWidthHeight){
        int width = newWidthHeight.first;
        int height = newWidthHeight.second;
        aspect = width / height;
        Projection = glm::perspective(FOV, aspect, zNear, zFar);
    });
}


Application::~Application() {
    mainShader->deleteProgram();
}


void Application::start() {
    //There must be a current window before OpenGL initialisation
    mainWindow.makeCurrent();
    mainWindow.getResizeNotifierPtr()->addListener(&onWindowResizeProjectionUpdater);

    //OpenGL initialisation
    if(gl3wInit()) {
        std::cerr << "Failed to initialize OpenGl.\n";
        throw GlfwInitalisationFailedException();
    }

    std::cout << "\tVendor:\t" << glGetString(GL_VENDOR) << std::endl;
    std::cout << "\tRenderer:\t" << glGetString(GL_RENDERER) << std::endl;
    std::cout << "\tOpenGL version:\t" << glGetString(GL_VERSION) << std::endl;
    std::cout << "\tGLSL version:\t" << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    glGetIntegerv(GL_MAX_PATCH_VERTICES, &maxPatchVerticies);
    std::cout << "\tTesselation Shader: Max supported patch verticies " <<  maxPatchVerticies << std::endl;

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(MessageCallback, 0);

    glClearColor(0.1,0.1,0.1,0);
    glfwSwapInterval(1);

    //Shader loading and consolidation
    try {
        mainShader->loadFromFile(Shader::VERTEX, shadersPath + "divide.vs");
        mainShader->loadFromFile(Shader::TESSELATION_CONTROL, shadersPath + "divide.tcs");
        mainShader->loadFromFile(Shader::TESSELATION_EVALUATION, shadersPath + "divide.tes");
        mainShader->loadFromFile(Shader::FRAGMENT, shadersPath + "divide.fs");
        mainShader->createAndLinkProgram();
        mainShader->use();
        mainShader->addAttribute("position");
        mainShader->addAttribute("color");
        mainShader->addUniform("ModelViewProjection");
        mainShader->addUniform("OuterTesselationLevel");
        mainShader->addUniform("InnerTesselationLevel");
        mainShader->unuse();
    } catch( MyException& e) {
        std::cerr << e.getType() << ":\n" << e.getMessage();
        throw e;
    }
    //TODO: ask for proper path if sth goes wrong

    initialiseGPUSenders();

    mainLoop();
}

void Application::initialiseGPUSenders() {
    initialiseAttributeSender();
    initialiseUniformSender();
}

void Application::initialiseUniformSender() {
    toGPUniformSender.addShader(mainShader);

    toGPUniformSender.addUniform(mainShader,
                                 UniformSendingInfo::MatrixType::FOUR,
                                 glm::value_ptr(ModelViewProjection),
                                 mainShader->getUniform("ModelViewProjection"));

    toGPUniformSender.addUniform(mainShader,
                                 UniformSendingInfo::UniformType::UNIFORM_FLOAT,
                                 &OuterTesselationLevel,
                                 mainShader->getUniform("OuterTesselationLevel"),
                                 1);

    toGPUniformSender.addUniform(mainShader,
                                 UniformSendingInfo::UniformType::UNIFORM_FLOAT,
                                 &InnerTesselationLevel,
                                 mainShader->getUniform("InnerTesselationLevel"),
                                 1);
}

void Application::initialiseAttributeSender() {
    toGPUattribueSender.createBuffers();

    toGPUattribueSender.addShader(mainShader);

    toGPUattribueSender.addAttribute(mainShader,
            mainShader->getAttribute("position"),
            3,
            GL_FLOAT,
            Vertex::getPositionOffset()
            );

    toGPUattribueSender.addAttribute(mainShader,
            mainShader->getAttribute("color"),
            3,
            GL_FLOAT,
            Vertex::getColorOffset()
            );
}

void Application::mainLoop() {
    insertGeometryAndTopologyIntoBuffers();
    while(mainWindow.isRunning()){
        Render();
        glfwPollEvents();
    }
}

void Application::insertGeometryAndTopologyIntoBuffers() {

    toGPUattribueSender.sendGeometryAndTopology(mainShader, mesh);

    /*
    GLsizei stride = mesh.getVerticiesStride();

    size_t indicesSize = mesh.getIndiciesSizeInBytes();
    size_t verticesSize = mesh.getVerticiesSizeInBytes();
    size_t positionOffset = mesh.getPositionOffset();
    size_t colorOffset = mesh.getColorOffset();

    GLuint positionLocation = static_cast<GLuint>(mainShader->getAttribute("position"));
    GLuint colorLocation = static_cast<GLuint>(mainShader->getAttribute("color"));

    glBindVertexArray(vaoId);

    glBindBuffer(GL_ARRAY_BUFFER, vboVerticesId);
    glBufferData(GL_ARRAY_BUFFER, verticesSize, mesh.getVerticiesDataPtr(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(positionLocation);
    glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(positionOffset));

    glBufferData(GL_ARRAY_BUFFER, verticesSize, mesh.getVerticiesDataPtr(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(colorLocation);
    glVertexAttribPointer(colorLocation, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(colorOffset));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndiciesId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, mesh.getIndiciesDataPtr(), GL_STATIC_DRAW);*/
}

void Application::Render() {
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    mainShader->use();

    toGPUniformSender.sendUniforms(mainShader);

    GLuint numIndicies = mesh.getNumberIndicies();

    glPatchParameteri(GL_PATCH_VERTICES, 3);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDrawElements(GL_PATCHES, numIndicies, GL_UNSIGNED_SHORT, nullptr);

    mainShader->unuse();
    mainWindow.swapBuffers();
}

void Application::MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
                                  const GLchar *message, const void *userParam) {
    fprintf( stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
             ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ),
             type, severity, message );
    if(type == GL_DEBUG_TYPE_ERROR)
        throw new OpenGlException();
}