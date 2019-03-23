//
// Created by dominik on 23.02.19.
//

#ifndef GAMEENGINE_VERTEX_H
#define GAMEENGINE_VERTEX_H

#include <glm/glm.hpp>

//Object data
class Vertex{
public:
    glm::vec3 position;
    glm::vec3 color;

    Vertex(const glm::vec3 &color, const glm::vec3 &position) : color(color), position(position) {}

    static auto getPositionOffset(){
        return offsetof(Vertex, position);
    }

    static auto getColorOffset(){
        return offsetof(Vertex, color);
    }
};

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#endif //GAMEENGINE_VERTEX_H
