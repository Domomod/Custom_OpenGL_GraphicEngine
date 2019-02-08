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
    GLuint program = 0;
    int totalShaders = 0;
    GLuint shaders[MAX_SHADERS];

    std::map<std::string, GLint> attributes;
    std::map<std::string, GLint> uniforms;
public:
    Shader();

    virtual ~Shader();

    void loadFromString(GLenum glShaderType, const std::string& sourceCode){
        if(sourceCode.size() != 0) {
            GLuint shader = glCreateShader(glShaderType);

            const char *source_c_str = sourceCode.c_str();
            glShaderSource(shader, 1, &source_c_str, NULL);
            glCompileShader(shader);

            GLint success = 0;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (success == GL_FALSE) {
                GLint compilationLogSize = 0;
                glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &compilationLogSize);

                std::vector<GLchar> compilationLog(compilationLogSize);
                glGetShaderInfoLog(shader, compilationLogSize, &compilationLogSize, &compilationLog[0]);

                std::string msg(compilationLog.begin(), compilationLog.end());

                throw ShaderCompilationFailedException(msg);
            }
            shaders[totalShaders] = shader;
            totalShaders++;
        }
        else if (sourceCode.size() == 0){
            throw EmptyFileException("Empty shader source code");
        }
    }

    void loadFromFile(GLenum glShaderType, const std::string& filePath){
        std::ifstream file(filePath);
        if (file.is_open()) {
            std::stringstream fileBuffer;
            fileBuffer << file.rdbuf();
            std::string sourceCode = fileBuffer.str();
            loadFromString(glShaderType, sourceCode);
        }
        else {
            throw EmptyFileException(filePath + " is an invalid path (no such file)");
        }
    }

    void createAndLinkProgram(){
        if(program == 0 && totalShaders != 0){
            program = glCreateProgram();
            for(int i = 0; i < totalShaders; i++){
                glAttachShader(program, shaders[i]);
            }
            GLint status;
            glLinkProgram(program);
            glGetProgramiv(program, GL_LINK_STATUS, &status);
            if(status == GL_FALSE){
                GLint consolidationLogSize;

                glGetProgramiv (program, GL_INFO_LOG_LENGTH, &consolidationLogSize);

                // The maxLength includes the NULL character
                std::vector<GLchar> consolidationLog(consolidationLogSize);
                glGetProgramInfoLog(program, consolidationLogSize, &consolidationLogSize, &consolidationLog[0]);

                std::string msg(consolidationLog.begin(), consolidationLog.end());

                throw ProgramConsolidationFailedException(msg);
            }
            for(int i = 0; i < totalShaders; i++){
                glDeleteShader(shaders[i]);
            }
        }
        else if (program != 0){
            throw GluintIdAlreadySetException("Program already created and linked");
        } else if (totalShaders = 0){
            throw ShadersNotCreatedException("You schould load at least one shaders before linking program.");
        }
    }

    void use(){

    }

    void unuse(){

    }

    void addAttribute(const std::string& attributeName){
        if(program != 0) {
            GLint attributeLocation = glGetAttribLocation(program, attributeName.c_str());
            if(attributeLocation != - 1) {
                attributes.emplace(attributeName, attributeLocation);
            } else{
                throw NoSuchAttributeException(attributeName + " is not an attribute in this glsl program.");
            }
        }
        else {
            throw NullProgramException("Can't add an attribute if program is not created and linked\n");
        }
    }

    void addUniform(const std::string& uniformName){
        if(program != 0) {
            GLint uniformLocation = glGetUniformLocation(program, uniformName.c_str());
            if(uniformLocation != - 1) {
                uniforms.emplace(uniformName, uniformLocation);
            } else{
                throw NoSuchAttributeException(uniformName + " is not an attribute in this glsl program.");
            }
        }
        else {
            throw NullProgramException("Can't add an attribute if program is not created and linked\n");
        }
    }

    void deleteProgram(){

    }
};
#endif //GAMEENGINE_SHADER_H
