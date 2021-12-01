#include "GPI_ShaderCompiler.h"
#include "glad/glad.h"

#include <iostream>

uint32_t GPI_CreateShader(const char* src, uint32_t TYPE)
{
    uint32_t shaderID = glCreateShader(TYPE);
    glShaderSource(shaderID, 1, &src, NULL);
    glCompileShader(shaderID);

    int32_t logStatus;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &logStatus);
    if(logStatus != GL_TRUE)
    {
        int32_t logLen;
        glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &logLen);
        char* errorMessage = (char*)alloca(logLen * sizeof(char));
        glGetShaderInfoLog(shaderID, logLen, &logLen, errorMessage);
        std::cout << errorMessage << std::endl;
        glDeleteShader(shaderID);
        return 0;
    }
    return shaderID;
}

uint32_t GPI_CreateProgram(uint32_t vertexShaderID, uint32_t fragmentShaderID)
{
    uint32_t programID = glCreateProgram();
    glAttachShader(programID, vertexShaderID);
    glAttachShader(programID, fragmentShaderID);
    glLinkProgram(programID);

    int32_t logStatus;
    glGetProgramiv(programID, GL_COMPILE_STATUS, &logStatus);
    if(logStatus != GL_TRUE)
    {
        int32_t logLen;
        glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &logLen);
        char* errorMessage = (char*)alloca(logLen * sizeof(char));
        glGetProgramInfoLog(programID, logLen, &logLen, errorMessage);
        std::cout << errorMessage << std::endl;
        glDeleteProgram(programID);
        return 0;
    }
    return programID;
}
