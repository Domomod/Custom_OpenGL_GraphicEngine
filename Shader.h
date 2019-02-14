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

#include "MyExceptions.h"

class Shader {
    //TODO: move private at the bottom of the class, after cleanup
private:
    enum ShaderType {VERTEX, TESSELATION_CONTROL, TESSELATION_EVALUATION, GEOMETRY, FRAGMENT, MAX_SHADERS};
    std::map<GLenum, ShaderType> GlenumShaderTypeConvert = {
            {GL_VERTEX_SHADER, VERTEX},
            {GL_TESS_CONTROL_SHADER, TESSELATION_CONTROL},
            {GL_TESS_EVALUATION_SHADER, TESSELATION_EVALUATION},
            {GL_GEOMETRY_SHADER, GEOMETRY},
            {GL_FRAGMENT_SHADER, FRAGMENT}
    };
    GLuint program = 0;
    int totalShaders = 0;
    GLuint shaders[MAX_SHADERS];

    void abstractAdd(const std::string& elementName, std::map<std::string, GLint>& elementDestination, GLint (*GLgetFunctionPtr)(GLuint, const GLchar *));

    std::map<std::string, GLint> attributes;
    std::map<std::string, GLint> uniforms;
public:
    Shader();

    virtual ~Shader();

    void loadFromString(GLenum glShaderType, const std::string& sourceCode);
    void loadFromFile(GLenum glShaderType, const std::string& filePath);
    void createAndLinkProgram();
    void deleteProgram();

    void use();
    void unuse();

    void addAttribute(const std::string& attributeName);
    void addUniform(const std::string& uniformName);

    GLint getAttribute(const std::string& attributeName);
    GLint getUniform(const std::string& uniformName);
    GLint abstractGet(const std::string &elementName, std::map<std::string, GLint> &elementStorage);
};
#endif //GAMEENGINE_SHADER_H
