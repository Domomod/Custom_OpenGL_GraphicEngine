//
// Created by dominik on 08.02.19.
//

#include "Shader.h"

#include "Source/MyExceptions.h"


Shader::Shader() {

}


Shader::~Shader() {

}


void Shader::loadFromString(ShaderType shaderType, const std::string &sourceCode) {
    if(sourceCode.size() != 0 ) {
        auto GlenumShaderType = GlenumShaderTypeConvert.at(shaderType);
        GLuint shader = glCreateShader(GlenumShaderType);

        const char *source_c_str = sourceCode.c_str();
        glShaderSource(shader, 1, &source_c_str, NULL);
        glCompileShader(shader);

        GLint success = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (success != GL_FALSE) {
            shaders[shaderType] = shader;
            loadedShaders[shaderType] = 1;
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


void Shader::loadFromFile(ShaderType shaderType, const std::string &filePath) {
    std::ifstream file(filePath);
    if (file.is_open()) {
        std::stringstream fileBuffer;
        fileBuffer << file.rdbuf();
        std::string sourceCode = fileBuffer.str();
        loadFromString(shaderType, sourceCode);
    }
    else {
        throw EmptyFileException(filePath + " is an invalid path (no such file)");
    }
}


void Shader::createAndLinkProgram() {
    if(program == 0 && loadedShaders[VERTEX]){
        program = glCreateProgram();
        for(int i = 0; i < MAX_SHADERS; i++){
            if(loadedShaders[i])
                glAttachShader(program, shaders[i]);
        }

        glBindFragDataLocation(program, 0, "fragColor");

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
        for(int i = 0; i < MAX_SHADERS; i++){
            if(loadedShaders[i])
                glDeleteShader(shaders[i]);
        }
        programLinked = true;
    }
    else if (program != 0){
        throw GluintIdAlreadySetException("Program already created and linked");
    } else if (!loadedShaders[VERTEX]){
        throw ShadersNotCreatedException("You schould load at least VERTEX shader before linking program.");
    }
}

void Shader::deleteProgram() {
    glUseProgram(0);
    glDeleteProgram(program);
    program = 0;
    programLinked = false;
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


GLuint Shader::getAttribute(const std::string &attributeName) {
    return static_cast<GLuint>(abstractGet(attributeName, attributes));
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

bool Shader::operator<(const Shader &rhs) const {
    return program < rhs.program;
}

bool Shader::operator>(const Shader &rhs) const {
    return rhs < *this;
}

bool Shader::operator<=(const Shader &rhs) const {
    return !(rhs < *this);
}

bool Shader::operator>=(const Shader &rhs) const {
    return !(*this < rhs);
}

bool Shader::operator==(const Shader &rhs) const {
    return program == rhs.program;
}

bool Shader::operator!=(const Shader &rhs) const {
    return !(rhs == *this);
}

GLuint Shader::getProgram() const {
    return program;
}
