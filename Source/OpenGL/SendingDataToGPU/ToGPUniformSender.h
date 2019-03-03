//
// Created by dominik on 27.02.19.
//

#ifndef GAMEENGINE_TOGPUUNIFORMSENDER_H
#define GAMEENGINE_TOGPUUNIFORMSENDER_H

#include <GL/gl3w.h>
#include <map>

#include "UniformSendingInfo.h"
#include "Source/OpenGL/ShaderProgram/Shader.h"

class ToGPUniformSender {
public:
    void addShader(std::shared_ptr<Shader> shader);

    void addUniform(const std::shared_ptr<Shader> &shader, UniformSendingInfo& info);

    template <class ... Type>
    void addUniform(const std::shared_ptr<Shader> &shader, Type ...  args){
        shaderToUniformInfoMap[shader].emplace_back(args...);
    }

    void sendUniforms(const std::shared_ptr<Shader> &shader);
private:
    //Uniforms are highly associated with shaders that use them,
    //thus the need to remember all shader's uniforms and their locations
    std::map<std::shared_ptr<Shader>, std::vector<UniformSendingInfo>> shaderToUniformInfoMap;
};


#endif //GAMEENGINE_TOGPUUNIFORMSENDER_H
