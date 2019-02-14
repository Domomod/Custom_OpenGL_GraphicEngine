//
// Created by dominik on 08.02.19.
//

#ifndef GAMEENGINE_SHADER_H
#define GAMEENGINE_SHADER_H

#include <GL/gl3w.h>
#include <string>
#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <memory>
#include <bitset>

#include "MyExceptions.h"

class Shader {
public:
    Shader();
    virtual ~Shader();

    enum ShaderType {VERTEX, TESSELATION_CONTROL, TESSELATION_EVALUATION, GEOMETRY, FRAGMENT, MAX_SHADERS};

    void loadFromString(ShaderType shaderType, const std::string& sourceCode);
    void loadFromFile(ShaderType shaderType, const std::string& filePath);
    void createAndLinkProgram();
    void deleteProgram();

    void use();
    void unuse();

    void addAttribute(const std::string& attributeName);
    void addUniform(const std::string& uniformName);

    GLint getAttribute(const std::string& attributeName);
    GLint getUniform(const std::string& uniformName);
    GLint abstractGet(const std::string &elementName, std::map<std::string, GLint> &elementStorage);

private:
    std::map<ShaderType, GLenum> GlenumShaderTypeConvert = {
            {VERTEX, GL_VERTEX_SHADER},
            {TESSELATION_CONTROL, GL_TESS_CONTROL_SHADER},
            {TESSELATION_EVALUATION, GL_TESS_EVALUATION_SHADER},
            {GEOMETRY, GL_GEOMETRY_SHADER},
            {FRAGMENT, GL_FRAGMENT_SHADER}
    };

    GLuint program = 0;
    GLuint shaders[MAX_SHADERS];
    std::bitset<MAX_SHADERS> loadedShaders;


    std::map<std::string, GLint> attributes;
    std::map<std::string, GLint> uniforms;

    void abstractAdd(const std::string& elementName, std::map<std::string, GLint>& elementDestination, GLint (*GLgetFunctionPtr)(GLuint, const GLchar *));

};
#endif //GAMEENGINE_SHADER_H
