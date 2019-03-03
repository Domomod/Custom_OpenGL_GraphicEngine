//
// Created by dominik on 23.02.19.
//

#ifndef GAMEENGINE_MESH_H
#define GAMEENGINE_MESH_H

#include <vector>
#include <GL/gl3w.h>

#include "Vertex.h"

class Mesh {
private:
    std::vector<Vertex> verticies;
    std::vector<GLushort> indicies;
public:
    Mesh() = default;
    Mesh(const std::vector<Vertex> &verticies, const std::vector<GLushort> &indicies);

    GLsizei getVerticiesStride() {
        return sizeof(verticies[0]);
    }

    GLuint getNumberIndicies(){
        return static_cast<GLuint>(indicies.size());
    }

    GLuint getNumberVerticies(){
        return static_cast<GLuint>(verticies.size());
    }

    size_t getIndiciesSizeInBytes(){
        return indicies.size() * sizeof(indicies[0]);
    }

    size_t getVerticiesSizeInBytes(){
        return verticies.size() * sizeof(verticies[0]);
    }

    size_t getPositionOffset(){
        return offsetof(Vertex, position);
    }

    size_t getColorOffset(){
        return offsetof(Vertex, color);
    }

    auto getVerticiesDataPtr() {
        return verticies.data();
    }

    auto getIndiciesDataPtr() {
        return indicies.data();
    }
};


#endif //GAMEENGINE_MESH_H
