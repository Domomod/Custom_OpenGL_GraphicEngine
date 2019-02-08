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
    void setMsg(std::string _msg);

    std::string getType();

    std::string getMessage();

    MyException() = default;

    MyException(std::string _exceptionName);

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

class EmptyFileException : public MyException {
public:
    EmptyFileException() : MyException("EmptyFileException") {}
};
#endif //GAMEENGINE_MYEXCEPTIONS_H
