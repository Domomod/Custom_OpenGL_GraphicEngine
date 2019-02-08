//
// Created by dominik on 08.02.19.
//

#ifndef GAMEENGINE_SHADER_H
#define GAMEENGINE_SHADER_H

#include <GL/gl3w.h>
#include <string>
#include <map>

class Shader {
public:
    Shader();
    ~Shader();

    void loadFromString(GLenum glShaderType, std::string& sourceCode){
    }

    void loadFromFile(GLenum glShaderType, std::string& filePath){

    }

    void createAndLinkProgram(){

    }

    void use(){

    }

    void unuse(){

    }

    void addAttribute(const std::string& attributeName){

    }

    void addUniform(const std::string& attributeName){

    }

    void deleteProgram(){

    }

private:
    enum ShaderType {VERTEX, TESSELATION_CONTROL, TESSELATION_EVALUATION, GEOMETRY, FRAGMENT, MAX_SHADERS};
    GLuint program;
    int totalShaders = 0;
    GLuint shaders[MAX_SHADERS];

    std::map<std::string, GLuint> attributes;
    std::map<std::string, GLuint> uniforms;

};


#endif //GAMEENGINE_SHADER_H
