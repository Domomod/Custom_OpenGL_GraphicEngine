//
// Created by dominik on 07.03.19.
//

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>

#include "Source/OpenGL/Window/Window.h"
#include "Source/OpenGL/OpenGLinitializer.h"
#include "Source/OpenGL/Window/Window.h"

#include "Source/OpenGL/RenderSystemV2/Buffers/VertexArrayObject.h"
#include "Source/OpenGL/RenderSystemV2/Buffers/AttributeBuffer.h"
#include "Source/OpenGL/RenderSystemV2/Buffers/ElementArrayBuffer.h"
#include "Source/OpenGL/RenderSystemV2/Metadata/AttributeMetadata.h"
#include "Source/OpenGL/RenderSystemV2/Buffers/UniformBuffer.h"

#include "Source/OpenGL/ShaderProgram/Shader.h"
#include "Source/EntitySystem/Mesh.h"
#include "Source/EntitySystem/MeshGenerator.h"

int main(){
    OpenGlInitalizer openGlInitalizer;
    openGlInitalizer.initGLFW();
    Window window;
    window.makeCurrent();
    openGlInitalizer.initOpenGL();
    Shader shader;

    std::vector<glm::vec3> positions = {
            glm::vec3(-1,-1,0),
            glm::vec3(0,1,0),
            glm::vec3(1,-1,0)
    };

    glm::vec4 color = glm::vec4(1,0,0,1);

    std::vector<GLushort> indicies = {
            0, 1, 2
    };

    try {
        shader.loadFromFile(Shader::VERTEX, "../Shaders/basic.vs");
        shader.loadFromFile(Shader::FRAGMENT, "../Shaders/basic.fs");
        shader.createAndLinkProgram();
        shader.use();
        shader.addAttribute("position");
        shader.unuse();
    } catch( MyException& e) {
        std::cerr << e.getType() << ":\n" << e.getMessage();
        throw e;
        //TODO: ask for proper path if sth goes wrong
    }

    VertexArrayObject vao;
    ElementArrayBuffer elementArrayBuffer;
    AttributeBuffer attributeBuffer;

    shader.use();
    vao.bind();

    attributeBuffer.bind();
    attributeBuffer.addAttributeMetadata(
            AttributeMetadata(static_cast<GLuint>(shader.getAttribute("position")), 3, GL_FLOAT,0,
                              sizeof(positions[0]))
    );
    attributeBuffer.enableAllAttribsAndSpecifyTheirOffsetsIfVaoBinded();
    attributeBuffer.sendBufferToGPUifVaoBinded(positions);

    elementArrayBuffer.bind();
    elementArrayBuffer.sendIfVaoEnabled(indicies);

    UniformBuffer uniformBuffer = UniformBufferFactory()
                                    .setBinding(0)
                                    .insert( UniformMetadata( &color, GL_FLOAT_VEC4 ) )
                                    .make();

    uniformBuffer.bind();
    uniformBuffer.bakeData();
    uniformBuffer.sendBufferToGPU();
    uniformBuffer.unbind();

    while(window.isRunning()){
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, nullptr);
        glfwPollEvents();
        window.swapBuffers();
    }
    shader.unuse();
    elementArrayBuffer.unbind();
    attributeBuffer.unbind();
    vao.unbind();
    return 0;
}