//
// Created by dominik on 27.02.19.
//

#include "ForwardSinusWaveRenderer.h"

ForwardSinusWaveRenderer::ForwardSinusWaveRenderer() {}

ForwardSinusWaveRenderer::~ForwardSinusWaveRenderer() {
    shader->deleteProgram();
}