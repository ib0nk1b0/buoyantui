#ifndef UTILS_H
#define UTILS_H

#define CUTILS_IMPLEMENTATION
#define CUTILS_NO_PREFIX
#include "cutils.h"

// TODO: TEMP. find better place for this

GLenum glCheckError_(const char *file, int line)
{
    GLenum errorCode;
    while ((errorCode = glGetError()) != GL_NO_ERROR)
    {
        switch (errorCode)
        {
            case GL_INVALID_ENUM:                  printf("INVALID_ENUM                  | %d | %s\n", line, file); break;
            case GL_INVALID_VALUE:                 printf("INVALID_VALUE                 | %d | %s\n", line, file); break;
            case GL_INVALID_OPERATION:             printf("INVALID_OPERATION             | %d | %s\n", line, file); break;
            case GL_STACK_OVERFLOW:                printf("STACK_OVERFLOW                | %d | %s\n", line, file); break;
            case GL_STACK_UNDERFLOW:               printf("STACK_UNDERFLOW               | %d | %s\n", line, file); break;
            case GL_OUT_OF_MEMORY:                 printf("OUT_OF_MEMORY                 | %d | %s\n", line, file); break;
            case GL_INVALID_FRAMEBUFFER_OPERATION: printf("INVALID_FRAMEBUFFER_OPERATION | %d | %s\n", line, file); break;
        }
    }
    return errorCode;
}
#define glCheckError(x) x; glCheckError_(__FILE__, __LINE__)


#endif // UTILS_H
