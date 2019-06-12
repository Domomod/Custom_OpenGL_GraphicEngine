// Created by dominik on 23.02.19.
//

#ifndef GAMEENGINE_MESH_H
#define GAMEENGINE_MESH_H

#include <vector>
#include <GL/gl3w.h>
#include <glm/glm.hpp>
#include <memory>

#include "Platform/OpenGL/Buffers/VertexArrayObject.h"
#include "Platform/OpenGL/Buffers/SingleAtributeBuffer.h"
#include "Platform/OpenGL/Buffers/ElementArrayBuffer.h"

class Texture2D;

class MaterialsLoader;

class Mesh
{
public:
    Mesh() = default;

    std::string name;

    Mesh(const std::string &name,
         const std::vector<glm::vec3> &positions, const std::vector<glm::vec4> &colors,
         const std::vector<glm::vec2> &uvs, const std::vector<glm::vec3> &normals,
         const std::vector<glm::vec3> &tangents, const std::vector<glm::ivec4> &boneIds,
         const std::vector<glm::vec4> &boneWeights, const std::vector<GLushort> &indicies,

         const std::shared_ptr<Texture2D> &normalMap, const std::shared_ptr<Texture2D> &aoMap,
         const std::shared_ptr<Texture2D> &albedoMap, const std::shared_ptr<Texture2D> &metallnessMap,
         const std::shared_ptr<Texture2D> &roughnessMap);

    void bindVao()
    {
        vao.bind();
    }

    void bindTexturesPBR();

    void bindPositon();

    void bindNormals();

    void bindTangents();

    void bindColors();

    void bindUvs();

    void bindBoneInfo();

    void unbindPositon();

    void unbindNormals();

    void unbindTangents();

    void unbindColors();

    void unbindUvs();

    void unbindBoneInfo();


    void setMaterial(MaterialsLoader &materialsLoader);

    unsigned int getIndiciesCount() const;

private:
    VertexArrayObject vao;

    SingleAttributeBuffer<0, glm::vec3> positionsB;
    SingleAttributeBuffer<1, glm::vec3> normalsB;
    SingleAttributeBuffer<2, glm::vec3> tangentsB;

    SingleAttributeBuffer<3, glm::vec4> colorsB;
    SingleAttributeBuffer<4, glm::vec2> uvB;
    SingleAttributeBuffer<5, glm::ivec4> boneIdsB;
    SingleAttributeBuffer<6, glm::vec4> boneWeightsB;
    ElementArrayBuffer indiciesB;

    unsigned int indiciesCount;

    std::shared_ptr<Texture2D> normalMap;
    std::shared_ptr<Texture2D> aoMap;
    std::shared_ptr<Texture2D> albedoMap;
    std::shared_ptr<Texture2D> metallnessMap;
    std::shared_ptr<Texture2D> roughnessMap;

    bool hasColors;
    bool hasUvs;
    bool hasTangents;
    bool hasBones;
};


#endif //GAMEENGINE_MESH_H