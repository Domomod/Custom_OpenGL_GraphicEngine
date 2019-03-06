//
// Created by dominik on 27.02.19.
//

#include "ForwardSinWaveRenderer.h"

void ForwardSinWaveRenderer::render(Mesh &mesh) {
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    shader->use();

    toGPUniformSender->sendUniforms(shader);

    GLuint numIndicies = mesh.getNumberIndicies();

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDrawElements(GL_TRIANGLES, numIndicies, GL_UNSIGNED_SHORT, nullptr);

    shader->unuse();
}

void ForwardSinWaveRenderer::setTime(GLfloat time) {
    ForwardSinWaveRenderer::time = time;
}

void ForwardSinWaveRenderer::setCenter(const glm::vec3 &center) {
    ForwardSinWaveRenderer::center = center;
}
