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

//TODO: move at the bottom of the file, after cleanup

class MyException : public std::exception {
public:
    void setMsg(std::string _msg){
        msg = _msg;
    }

    std::string getType(){
        return exceptionName;
    }

    std::string getMessage() {
        return "Shader compilation was unsuccessful, error message:\n" + msg + "\n";
    }

    MyException() {

    }

    MyException(std::string _exceptionName) : exceptionName(_exceptionName){}

private:
    std::string exceptionName;
    std::string msg = "No message was set";
};

class ShaderCompilationFailedException : public MyException {
public:
    ShaderCompilationFailedException() : MyException("ShaderCompilationFailedException") {}
};
class GluintIdAlreadySetException : public MyException {
public:
    GluintIdAlreadySetException() : MyException("GluintIdAlreadySetException") {}
};
class ShadersNotCreatedException : public MyException {
public:
    ShadersNotCreatedException() : MyException("ShadersNotCreatedException") {}
};



class Shader {
public:
    Shader();

    virtual ~Shader();

    void loadFromString(GLenum glShaderType, const std::string& sourceCode){
        GLuint shader = glCreateShader(glShaderType);

        const char *source_c_str = sourceCode.c_str();
        glShaderSource(shader, 1, &source_c_str, NULL);
        glCompileShader(shader);

        GLint success = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if(success == GL_FALSE){
            GLint compilationLogSize = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &compilationLogSize);
            GLsizei bytesPassed = 0;
            GLchar* compilationLog;
            glGetShaderInfoLog(shader, compilationLogSize, &bytesPassed, compilationLog);

            auto e = new ShaderCompilationFailedException();
            if(compilationLog != NULL){
                e->setMsg(compilationLog);
            }
            throw e;
        }
        shaders[totalShaders] = shader;
        totalShaders++;
    }

    void loadFromFile(GLenum glShaderType, const std::string& filePath){
        std::ifstream file(filePath);
        std::stringstream fileReader;
        fileReader << file.rdbuf();
        std::string sourceCode = fileReader.str();
        loadFromString(glShaderType, sourceCode);
    }

    void createAndLinkProgram(){
        if(program == 0 && totalShaders != 0){
            program = glCreateProgram();
            for(int i = 0; i < totalShaders; i++){
                glAttachShader(program, shaders[i]);
            }
        } else if (program != 0){
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
