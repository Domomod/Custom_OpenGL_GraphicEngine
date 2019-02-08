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

//TODO: move at the bottom of the file, after cleanup

#include "MyExceptions.h"

class Shader {
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

                // The maxLength includes the NULL character
                std::vector<GLchar> compilationLog(compilationLogSize);
                glGetShaderInfoLog(shader, compilationLogSize, &compilationLogSize, &compilationLog[0]);

                std::string msg(compilationLog.begin(), compilationLog.end());

                auto e = new ShaderCompilationFailedException();
                if (compilationLogSize != 0) {
                    e->setMsg(msg);
                }
                throw e;
            }
            shaders[totalShaders] = shader;
            totalShaders++;
        }
        else if (sourceCode.size() == 0){
            auto e = new EmptyFileException();
            e->setMsg("Empty shader source code");
            throw e;
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
            auto e = new EmptyFileException();
            e->setMsg(filePath + " is an invalid path (no such file)");
            throw e;
        }
    }

    void createAndLinkProgram(){
        if(program == 0 && totalShaders != 0){
            program = glCreateProgram();
            for(int i = 0; i < totalShaders; i++){
                glAttachShader(program, shaders[i]);
            }
        }
        else if (program != 0){
            auto e = new GluintIdAlreadySetException();
            e->setMsg("Program already created and linked");
            throw e;
        } else if (totalShaders = 0){
            auto e = new ShadersNotCreatedException();
            e->setMsg("You schould load at least one shaders before linking program.");
            throw e;
        }
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
    GLuint program = 0;
    int totalShaders = 0;
    GLuint shaders[MAX_SHADERS];

    std::map<std::string, GLuint> attributes;
    std::map<std::string, GLuint> uniforms;

};
#endif //GAMEENGINE_SHADER_H
