#include <utility>

//
// Created by dominik on 08.02.19.
//

#ifndef GAMEENGINE_MYEXCEPTIONS_H
#define GAMEENGINE_MYEXCEPTIONS_H

#include <GL/freeglut.h>
#include <string>
#include <iostream>

class MyException : public std::exception {
public:
    std::string getType();

    std::string getMessage();

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
    GlfwInitalisationFailedException(const std::string &msg = "\n") : MyException("GlfwInitalisationFailedException", msg) {}

};

class OpenGlException : public  MyException{
public:
    OpenGlException(const std::string &msg = "\n") : MyException("OpenGlException", msg) {}
};

class InvalidData : public MyException{
public:
    InvalidData(const std::string &msg = "\n") : MyException("InvalidData", msg) {}

};
#endif //GAMEENGINE_MYEXCEPTIONS_H
