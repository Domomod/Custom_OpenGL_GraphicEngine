//
// Created by dominik on 08.02.19.
//

#include "MyExceptions.h"
#include <string>

std::string MyException::getType() {
    return exceptionName;
}

void MyException::setMsg(std::string _msg) {
    msg = std::move(_msg);
}

std::string MyException::getMessage() {
    return "Shader compilation was unsuccessful, error message:\n" + msg + "\n";
}

MyException::MyException(std::string _exceptionName) : exceptionName(std::move(_exceptionName)){}
