//
// Created by dominik on 06.03.19.
//

#ifndef GAMEENGINE_RENDERER_H
#define GAMEENGINE_RENDERER_H

#include <memory>
#include <vector>

#include "Source/OpenGL/RenderSystemV2/ContentProvider/RenderContentProvider.h"
#include "Source/OpenGL/RenderSystemV2/Buffers/UniformBuffer.h"

/*Renderer cares to send its uniforms
 * */
class Renderer {
protected:
    std::vector<std::shared_ptr<RenderContentProvider>> contentProviders;
    std::shared_ptr<UniformBuffer> uniformBuffer;
public:
    virtual void setContext() = 0;
    virtual void resetContext() = 0;
    void render(){
        setContext();
        for(const auto &contentProvider : contentProviders){
              contentProvider->drawContent();
        }
    };

    bool sharesUniformsWith(Renderer& renderer){
        return uniformBuffer == renderer.uniformBuffer;
    }
};


#endif //GAMEENGINE_RENDERER_H
