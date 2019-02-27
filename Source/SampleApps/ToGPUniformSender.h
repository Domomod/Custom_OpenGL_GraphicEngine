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

    std::map<Shader*, std::vector<UniformSendingInfo>> shaderToUniformInfoMap;
public:
    void addShader(Shader* shader){
        shaderToUniformInfoMap.emplace(shader,std::vector<UniformSendingInfo>());
    }

    void addUniform(Shader* shader, UniformSendingInfo& info){
        shaderToUniformInfoMap[shader].push_back(info);
    }

    template <class ... Type>
    void addUniform(Shader* shader, Type ...  args){
        shaderToUniformInfoMap[shader].push_back(UniformSendingInfo(args...));
    }

    void sendUniforms(Shader* shader){
        for(auto& info : shaderToUniformInfoMap[shader]){
            info.Send();
        }
    }
};


#endif //GAMEENGINE_TOGPUUNIFORMSENDER_H
