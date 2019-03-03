//
// Created by dominik on 27.02.19.
//

#include "ToGPUniformSender.h"

void ToGPUniformSender::addShader(std::shared_ptr<Shader> shader) {
    shaderToUniformInfoMap.emplace(shader,std::vector<UniformSendingInfo>());
}

void ToGPUniformSender::addUniform(const std::shared_ptr<Shader> &shader, UniformSendingInfo &info) {
    shaderToUniformInfoMap[shader].push_back(info);
}

void ToGPUniformSender::sendUniforms(const std::shared_ptr<Shader> &shader) {
    for(auto& info : shaderToUniformInfoMap[shader]){
        info.Send();
    }
}
