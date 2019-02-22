//
// Created by dominik on 08.02.19.
//

#include "MyExceptions.h"
#include <string>

std::string MyException::getType() {
    return exceptionName;
}

std::string MyException::getMessage() {
    return "Shader compilation was unsuccessful, error message:\n" + msg + "\n";
}

MyException::MyException(std::string _exceptionName) : exceptionName(std::move(_exceptionName)){}

MyException::MyException(const std::string &exceptionName, const std::string &msg = "No message was set") : exceptionName(exceptionName),
                                                                                     msg(msg) {}
