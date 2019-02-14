//
// Created by dominik on 08.02.19.
//

#include "Shader.h"

Shader::Shader() {

}


Shader::~Shader() {

}


void Shader::loadFromString(GLenum glShaderType, const std::string &sourceCode) {
    if(sourceCode.size() != 0 ) {

        GLuint shader = glCreateShader(glShaderType);

        const char *source_c_str = sourceCode.c_str();
        glShaderSource(shader, 1, &source_c_str, NULL);
        glCompileShader(shader);

        GLint success = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (success != GL_FALSE) {
            ShaderType shaderType = GlenumShaderTypeConvert.at(glShaderType);
            shaders[shaderType] = shader;
            totalShaders++;
        } else {
            GLint compilationLogSize = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &compilationLogSize);

            std::vector<GLchar> compilationLog(compilationLogSize);
            glGetShaderInfoLog(shader, compilationLogSize, &compilationLogSize, &compilationLog[0]);

            std::string msg(compilationLog.begin(), compilationLog.end());

            throw ShaderCompilationFailedException(msg);
        }
    }
    else if (sourceCode.size() == 0) {
        throw EmptyFileException("Empty shader source code");
    }
}


void Shader::loadFromFile(GLenum glShaderType, const std::string &filePath) {
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


void Shader::createAndLinkProgram() {
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


void Shader::use() {
    glUseProgram(program);
}


void Shader::unuse() {
    glUseProgram(0);
}


void Shader::addAttribute(const std::string &attributeName) {
    abstractAdd(attributeName, attributes, glGetAttribLocation);
}


void Shader::addUniform(const std::string &uniformName) {
    abstractAdd(uniformName, uniforms, glGetUniformLocation);
}


void Shader::abstractAdd(const std::string& elementName, std::map<std::string, GLint>& elementDestination, GLint (*GLgetFunctionPtr)(GLuint, const GLchar *)){
    if(program != 0) {
        GLint elementLocation = GLgetFunctionPtr(program, elementName.c_str());
        if(elementLocation != - 1) {
            elementDestination.emplace(elementName, elementLocation);
        } else{
            throw NoSuchGlslElementException(elementName + " is not an attribute in this glsl program.");
        }
    }
    else {
        throw NullProgramException("Can't add an attribute if program is not created and linked\n");
    }
}


GLint Shader::getAttribute(const std::string &attributeName) {
    return abstractGet(attributeName, attributes);
}


GLint Shader::getUniform(const std::string &uniformName) {
    return abstractGet(uniformName, uniforms);
}


GLint Shader::abstractGet(const std::string &elementName, std::map<std::string, GLint> &elementStorage) {
    GLint elementId;
    try{
        elementId = elementStorage[elementName];
    } catch (std::out_of_range& e) {
        throw NoSuchGlslElementException(elementName + "was not added to shader's uniforms");
    }
    return elementId;
}


void Shader::deleteProgram() {
    glDeleteProgram(program);
}