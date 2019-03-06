//
// Created by dominik on 06.03.19.
//

#ifndef GAMEENGINE_UNIFORMBUFFER_H
#define GAMEENGINE_UNIFORMBUFFER_H


#include <cstdio>
#include <GL/gl3w.h>
#include <map>
#include <vector>

#include "Source/OpenGL/ShaderProgram/Shader.h"

/*Class is not checking validity of passed ptrs, their type has
 * to match the type of uniforms in shader codes*/

class UniformBuffer {
public:
    void acquire(std::string& bufferName, Shader& shader);
    void acquire(GLuint _binding, Shader& shader);
    void release();
    void addUniform(std::string name, void *data, size_t size);

    GLubyte* getBakedData();

    UniformBuffer() = default;
    virtual ~UniformBuffer();
private:
    void retrieveBlockSizeFromProgram(GLuint program);
    void retrieveUniformOrderFromProgram(GLuint program);
    void retrieveUniformOffsetsFromProgram(GLuint program);
    void allocateBytesTable();
    void releaseBytesTable();
    void bakeData();

    GLuint binding;
    GLint blockSize;
    std::vector<GLchar*> uniformNames;

    //This can be an outer class
    std::vector<void*> uniformData;
    std::vector<size_t> uniformSizes;
    std::vector<GLuint> uniformOrder;
    std::vector<GLint> uniformOffsets;
    // For example UniformMetadata

    GLubyte* bytesTable;
    bool isAcquired = false;
};


#endif //GAMEENGINE_UNIFORMBUFFER_H
