//
// Created by dominik on 26.02.19.
//

#ifndef GAMEENGINE_TOGPUDATALOADER_H
#define GAMEENGINE_TOGPUDATALOADER_H

#include <GL/gl3w.h>
#include <map>

#include "Mesh.h"
#include "Source/View/Shader.h"
#include "Source/MyExceptions.h"
#include "Source/SampleApps/VertexLoadingInfo.h"


class ToGPUattribueSender {

public:
    ToGPUattribueSender() = default;

    virtual ~ToGPUattribueSender();

    void createBuffers();

    void addShader(std::shared_ptr<Shader> shader);

    template<class ... Type>
    void addAttribute(std::shared_ptr<Shader> shader, Type ... args) {
        shaderToAttributeLoadingInfoMap[shader].emplace_back(args...);
    }

    void sendGeometryAndTopology(const std::shared_ptr<Shader>& shader, Mesh& mesh);

    void sendGeometry(const std::shared_ptr<Shader>& shader, Mesh& mesh);

    void sendTopology(Mesh& mesh);

private:
    std::map<std::shared_ptr<Shader>,std::vector<VertexLoadingInfo>> shaderToAttributeLoadingInfoMap;
    GLuint vaoId;
    GLuint vboVerticesId;
    GLuint vboIndiciesId;

};


#endif //GAMEENGINE_TOGPUDATALOADER_H
