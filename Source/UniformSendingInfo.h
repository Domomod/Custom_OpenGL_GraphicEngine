//
// Created by dominik on 27.02.19.
//

#ifndef GAMEENGINE_UNIFORMSENDINGINFO_H
#define GAMEENGINE_UNIFORMSENDINGINFO_H

#include <GL/gl3w.h>

#include "MyExceptions.h"

class UniformSendingInfo {
public:
    enum class UniformType {
        UNIFORM_FLOAT,
        UNIFORM_INTEGER,
        UNIFORM_UNSIGNED_INTEGER,
        UNIFORM_MATRIX
    };

    typedef void (*GLintLoadingFunction)( GLint location, GLsizei count, const GLint *value);
    typedef void (*GLunsignedIntLoadingFunction)( GLint location, GLsizei count, const GLuint *value);
    typedef void (*GLfloatLoadingFunction)( GLint location, GLsizei count, const GLfloat *value);
    typedef void (*GLmatrixLoadingFunction)( GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);

    const GLintLoadingFunction intLoadingFunctions[4] = {
            glUniform1iv, glUniform2iv, glUniform3iv, glUniform4iv
    };

    const GLunsignedIntLoadingFunction unsignedIntLoadingFunctions[4] = {
            glUniform1uiv, glUniform2uiv, glUniform3uiv, glUniform4uiv
    };

    const GLfloatLoadingFunction floatLoadingFunctions [4] = {
            glUniform1fv, glUniform2fv, glUniform3fv, glUniform4fv
    };

    //LOADING FUNCTIONS AND MATRIX TYPES NEED TO STAY STRICTLY IN THE SAME ORDER
    const GLmatrixLoadingFunction matrixLoadingFunctions [9] = {
            glUniformMatrix2fv,
            glUniformMatrix3fv,
            glUniformMatrix4fv,
            glUniformMatrix2x3fv,
            glUniformMatrix3x2fv,
            glUniformMatrix2x4fv,
            glUniformMatrix4x2fv,
            glUniformMatrix3x4fv,
            glUniformMatrix4x3fv
    };
    //LOADING FUNCTIONS AND MATRIX TYPES NEED TO STAY STRICTLY IN THE SAME ORDER
    enum class MatrixType : int{
        TWO,
        THREE,
        FOUR,
        TWO_BY_THREE,
        THREE_BY_TWO,
        TWO_BY_FOUR,
        FOUR_BY_TWO,
        THREE_BY_FOUR,
        FOUR_BY_THREE,
        NONE
    };
    //LOADING FUNCTIONS AND MATRIX TYPES NEED TO STAY STRICTLY IN THE SAME ORDER

    GLint location;
    GLsizei vectorSize;
    GLsizei elementSize;
    GLboolean transpose;
    const GLvoid *value;
    UniformType type;
    MatrixType matrixType;

    //elementSize = 1 ... n will cause invocation of 0-th ... (n-1)-th function in glFunction
    //thus we are saving elementSize -1 to optimize operations
    UniformSendingInfo(UniformType type, const void *value, GLint location, GLsizei elementSize, GLsizei vectorSize = 1
                       ) : location(location), vectorSize(vectorSize), elementSize(elementSize-1),
                                           transpose(GL_FALSE), value(value), type(type), matrixType(MatrixType::NONE) {
        if(type == UniformType::UNIFORM_MATRIX)
            throw InvalidData("Tried to create Matrix UniformSendingInfo, to create Matrix UniformSendingInfo use the other constructor");
    }

    UniformSendingInfo(MatrixType type, const void *value, GLint location, GLboolean transpose = GL_FALSE, GLsizei vectorSize = 1
            ) : location(location), vectorSize(vectorSize), elementSize(-1),
                transpose(transpose), value(value), type(UniformType::UNIFORM_MATRIX), matrixType(type) {
    }

    void Send(){
        switch(type){
            case UniformType::UNIFORM_INTEGER : {
                intLoadingFunctions[elementSize](location, vectorSize, reinterpret_cast<const GLint*>(value));
                break;
            }
            case UniformType::UNIFORM_UNSIGNED_INTEGER : {
                unsignedIntLoadingFunctions[elementSize](location, vectorSize, reinterpret_cast<const GLuint*>(value));
                break;
            }
            case UniformType::UNIFORM_FLOAT : {
                floatLoadingFunctions[elementSize](location, vectorSize, reinterpret_cast<const GLfloat *>(value));
                break;
            }
            case UniformType::UNIFORM_MATRIX : {
                matrixLoadingFunctions[static_cast<int>(matrixType)](location, vectorSize, transpose, reinterpret_cast<const GLfloat *>(value));
                break;
            }
        }
    };

};

#endif //GAMEENGINE_UNIFORMSENDINGINFO_H