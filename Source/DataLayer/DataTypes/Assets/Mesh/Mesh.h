// Created by dominik on 23.02.19.
//

#ifndef GAMEENGINE_MESH_H
#define GAMEENGINE_MESH_H

#include <vector>
#include <GL/gl3w.h>
#include <glm/glm.hpp>
#include <memory>

#include "Source/GraphicsLayer/RenderSystemV2/Buffers/VertexArrayObject.h"
#include "Source/GraphicsLayer/RenderSystemV2/Buffers/SingleAtributeBuffer.h"
#include "Source/GraphicsLayer/RenderSystemV2/Buffers/ElementArrayBuffer.h"

class Texture;

class Mesh {
public:
    Mesh() = default;
    std::string name;

    Mesh(const std::string &name,
         const std::vector<glm::vec3> &positions,   const std::vector<glm::vec4 > &colors,
         const std::vector<glm::vec2> &uvs,          const std::vector<glm::vec3 > &normals,
         const std::vector<glm::vec3> &tangents,    const std::vector<glm::ivec4> &boneIds,
         const std::vector<glm::vec4> &boneWeights, const std::vector<GLushort> &indicies,

         const std::shared_ptr<Texture> &normalMap, const std::shared_ptr<Texture> &aoMap,
         const std::shared_ptr<Texture> &albedoMap, const std::shared_ptr<Texture> &metallnessMap,
         const std::shared_ptr<Texture> &roughnessMap);

    void bindVao(){vao.bind();}
    void bindTexturesPBR();

    void bindPositon()  { positionsB.bind( vao.getVaoName());  }
    void bindNormals()  { normalsB.bind( vao.getVaoName());    }
    void bindTangents() { tangentsB.bind( vao.getVaoName());   }
    void bindColors()   { colorsB.bind( vao.getVaoName());     }
    void bindUvs()      { uvB.bind( vao.getVaoName());         }
    void bindBoneInfo() { boneIdsB.bind( vao.getVaoName());
                          boneWeightsB.bind( vao.getVaoName());}

    void unbindPositon() { positionsB.unbind( vao.getVaoName());  }
    void unbindNormals() { normalsB.unbind( vao.getVaoName());    }
    void unbindTangents(){ tangentsB.unbind( vao.getVaoName());   }
    void unbindColors()  { colorsB.unbind( vao.getVaoName());     }
    void unbindUvs()     { uvB.unbind( vao.getVaoName());         }
    void unbindBoneInfo(){ boneIdsB.unbind( vao.getVaoName());
                           boneWeightsB.unbind( vao.getVaoName());}

    unsigned int getIndiciesCount() const;

private:

    VertexArrayObject vao;

    SingleAttributeBuffer<0, glm::vec3 >  positionsB;
    SingleAttributeBuffer<1, glm::vec3 >  normalsB;
    SingleAttributeBuffer<2, glm::vec3 >  tangentsB;

    SingleAttributeBuffer<3, glm::vec4 >  colorsB;
    SingleAttributeBuffer<4, glm::vec2 >  uvB;
    SingleAttributeBuffer<5, glm::ivec4>  boneIdsB;
    SingleAttributeBuffer<6, glm::vec4 >  boneWeightsB;
    ElementArrayBuffer indiciesB;

    unsigned int indiciesCount;

    std::shared_ptr<Texture> normalMap;
    std::shared_ptr<Texture> aoMap;
    std::shared_ptr<Texture> albedoMap;
    std::shared_ptr<Texture> metallnessMap;
    std::shared_ptr<Texture> roughnessMap;

    bool hasColors;
    bool hasUvs;
    bool hasBones;
};


#endif //GAMEENGINE_MESH_H