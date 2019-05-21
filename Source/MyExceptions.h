#include <utility>

//
// Created by dominik on 08.02.19.
//

#ifndef GAMEENGINE_MYEXCEPTIONS_H
#define GAMEENGINE_MYEXCEPTIONS_H

#include <string>
#include <iostream>

class MyException : public std::exception {
public:
    std::string getType();

    std::string getMessage();

    const char* what() const noexcept { return msg.c_str(); };

    MyException() = default;
    MyException(std::string _exceptionName);
    MyException(const std::string &exceptionName, const std::string &msg);

private:
    std::string exceptionName;
    std::string msg = "No message was set";
};

//GLSL EXCEPTIONS
class ShaderCompilationFailedException : public MyException {
public:
    ShaderCompilationFailedException(const std::string &msg = "No message was set") : MyException("ShaderCompilationFailedException", msg) {}
};

class GluintIdAlreadySetException : public MyException {
public:
    GluintIdAlreadySetException(const std::string &msg = "No message was set") : MyException("GluintIdAlreadySetException", msg) {}
};

class ShadersNotCreatedException : public MyException {
public:
    ShadersNotCreatedException(const std::string &msg = "No message was set") : MyException("ShadersNotCreatedException", msg) {}
};

class IncorrectShaderTypeException : public MyException {
public:
    IncorrectShaderTypeException(const std::string &msg = "No message was set") : MyException("IncorrectShaderTypeException", msg) {}
};


class ProgramConsolidationFailedException : public MyException {
public:
    ProgramConsolidationFailedException(const std::string &msg = "No message was set") : MyException("ProgramConsolidationFailedException", msg) {}
};

class EmptyFileException : public MyException {
public:
    EmptyFileException(const std::string &msg = "No message was set") : MyException("EmptyFileException", msg) {}
};

class NullProgramException : public MyException {
public:
    NullProgramException(const std::string &msg = "No message was set") : MyException("NullProgramException", msg) {}
};

class NoSuchGlslElementException : public MyException {
public:
    NoSuchGlslElementException(const std::string &msg = "No message was set\nTip: check for misspelling errors.") : MyException("NoSuchUniformException", msg) {}
};

//OPENGL Exceptions
class GlfwInitalisationFailedException : public MyException{
public:
    GlfwInitalisationFailedException(const std::string &msg = "No message was set\n") : MyException("GlfwInitalisationFailedException", msg) {}

};

class OpenGlException : public  MyException{
public:
    OpenGlException(const std::string &msg = "No message was set\n") : MyException("OpenGlException", msg) {}
};

class InvalidData : public MyException {
public:
    InvalidData(const std::string &msg = "No message was set\n") : MyException("InvalidData", msg) {}
};

class InvalidImage : public MyException {
public:
    InvalidImage(const std::string &msg = "No message was set\n") : MyException("InvalidImage", msg) {}
};

//EntitySystemExceptions
class MeshNotFound : public MyException{
public:
    MeshNotFound(const std::string &msg = "No message was set\n") : MyException("MeshNotFound", msg) {}
};

class ModelLoadingException : public MyException{
public:
    ModelLoadingException(const std::string &msg = "No message was set\n", const std::string& name = "") : MyException("ModelLoadingException:\t"+name, msg) {}
};

class MeshLoadingException : public ModelLoadingException{
public:
    MeshLoadingException(const std::string &msg = "No message was set\n") : ModelLoadingException(msg, "MeshLoadingException") {}
};

class SkeletonLoadingException : public ModelLoadingException{
public:
    SkeletonLoadingException(const std::string &msg = "No message was set\n") : ModelLoadingException(msg, "SkeletonLoadingException") {}
};

class AnimationLoadingException : public ModelLoadingException{
public:
    AnimationLoadingException(const std::string &msg = "No message was set\n") : ModelLoadingException(msg, "AnimationLoadingException") {}
};

//SkeletalSystemExceptions
class AnimationInterpolationError : public MyException{
public:
    AnimationInterpolationError(const std::string &msg = "No message was set\n") : MyException("AnimationInterpolationError", msg) {}
};

//CommonExceptions
class FileNotFound : public MyException{
public:
    FileNotFound(const std::string &msg = "No message was set\n") : MyException("FileNotFound", msg) {}
};

//CommonExceptions
class TextureConverterShaderNotSet : public MyException{
public:
    TextureConverterShaderNotSet(const std::string &msg = "No message was set\n") : MyException("TextureConverterShaderNotSet", msg) {}
};

class InvalidOpenGLTypeException : public MyException{
public:
    InvalidOpenGLTypeException(const std::string &msg = "No message was set\n") : MyException("InvalidOpenGLTypeException", msg) {}
};
#endif //GAMEENGINE_MYEXCEPTIONS_H
