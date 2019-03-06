//
// Created by dominik on 02.03.19.
//

#ifndef GAMEENGINE_FORWARDRENDERER_H
#define GAMEENGINE_FORWARDRENDERER_H

#include "Source/OpenGL/RenderSystem/AbstractRenderer.h"

class ForwardAbstractRenderer : public AbstractRenderer{
public:
    ForwardAbstractRenderer(
            ) : AbstractRenderer(new ToGPUattribueSender(), new ToGPUniformSender()){}

    void init() override;

protected:
    virtual void initShaders() = 0;

    std::shared_ptr<Shader> shader = std::make_shared<Shader>();
};


#endif //GAMEENGINE_FORWARDRENDERER_H
