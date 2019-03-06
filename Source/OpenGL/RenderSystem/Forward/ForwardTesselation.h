//
// Created by dominik on 03.03.19.
//

#ifndef GAMEENGINE_FORWARDTESSELATION_H
#define GAMEENGINE_FORWARDTESSELATION_H

#include "Source/OpenGL/RenderSystem/Forward/ForwardAbstractRenderer.h"

class ForwardTesselation final : public ForwardAbstractRenderer{
public:
    ForwardTesselation() = default;

    virtual ~ForwardTesselation() = default;

    void init() override;

    void render(Mesh &mesh) override;

    void sendMeshDataToGPU(Mesh& mesh) override;

    void setOuterTesselationLevel(GLfloat OuterTesselationLevel);

    void setInnerTesselationLevel(GLfloat InnerTesselationLevel);

private:
    void initShaders() override;;

    void initialiseUniformSender() override;

    void initialiseAttributeSender() override;

    //Uniforms
    glm::mat4 Projection = glm::mat4(1);
    glm::mat4 ModelView = glm::mat4(1);
    glm::mat4 ModelViewProjection = glm::mat4(1);

    GLfloat OuterTesselationLevel = 3.0f;
    GLfloat InnerTesselationLevel = 3.0f;
};


#endif //GAMEENGINE_FORWARDTESSELATION_H
