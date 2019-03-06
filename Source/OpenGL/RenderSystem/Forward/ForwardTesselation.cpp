//
// Created by dominik on 03.03.19.
//

#include "ForwardTesselation.h"

void ForwardTesselation::render(Mesh &mesh) {
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    shader->use();

    toGPUniformSender->sendUniforms(shader);

    GLuint numIndicies = mesh.getNumberIndicies();

    glPatchParameteri(GL_PATCH_VERTICES, 3);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDrawElements(GL_PATCHES, numIndicies, GL_UNSIGNED_SHORT, nullptr);

    shader->unuse();
}

void ForwardTesselation::init() {
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

void ForwardTesselation::initShaders() {
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


void ForwardTesselation::initialiseUniformSender() {
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

void ForwardTesselation::initialiseAttributeSender() {
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

void ForwardTesselation::sendMeshDataToGPU(Mesh &mesh) {
    toGPUattribueSender->sendGeometryAndTopology(shader,mesh);
}

void ForwardTesselation::setOuterTesselationLevel(GLfloat OuterTesselationLevel) {
    ForwardTesselation::OuterTesselationLevel = OuterTesselationLevel;
}

void ForwardTesselation::setInnerTesselationLevel(GLfloat InnerTesselationLevel) {
    ForwardTesselation::InnerTesselationLevel = InnerTesselationLevel;
}