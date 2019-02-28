//
// Created by dominik on 27.02.19.
//

#ifndef GAMEENGINE_TOGPUUNIFORMSENDER_H
#define GAMEENGINE_TOGPUUNIFORMSENDER_H

#include <GL/gl3w.h>
#include <map>

#include "../UniformSendingInfo.h"
#include "../View/Shader.h"

class ToGPUniformSender {
private:
    void sendSingleUniform(UniformSendingInfo& info){

    }

    //Uniforms are highly associated with shaders that use them,
    //thus the need to remember all shader's uniforms and their locations
    std::map<std::shared_ptr<Shader>, std::vector<UniformSendingInfo>> shaderToUniformInfoMap;
public:
    void addShader(std::shared_ptr<Shader> shader){
        shaderToUniformInfoMap.emplace(shader,std::vector<UniformSendingInfo>());
    }

    void addUniform(std::shared_ptr<Shader> shader, UniformSendingInfo& info){
        shaderToUniformInfoMap[shader].push_back(info);
    }

    template <class ... Type>
    void addUniform(std::shared_ptr<Shader> shader, Type ...  args){
        shaderToUniformInfoMap[shader].push_back(UniformSendingInfo(args...));
    }

    void sendUniforms(std::shared_ptr<Shader> shader){
        for(auto& info : shaderToUniformInfoMap[shader]){
            info.Send();
        }
    }
};


#endif //GAMEENGINE_TOGPUUNIFORMSENDER_H
