#include "GPI_Shader.h"

#include "GPI_FileReader.h"
#include "GPI_ShaderCompiler.h"

#include "glad/glad.h"
#include <malloc.h>

#include <stdio.h>

GPI_Shader GPI_CreateShaderFromFiles(char* vertexPath,
                                            char* fragmentPath)
{
    char* vertexSrc = GPI_ReadFile(vertexPath);
    char* fragmentSrc = GPI_ReadFile(fragmentPath);

    uint32_t vertexShaderID = GPI_CreateShader(vertexSrc, GL_VERTEX_SHADER);
    uint32_t fragmentShaderID = GPI_CreateShader(fragmentSrc, GL_FRAGMENT_SHADER);

    uint32_t programID = GPI_CreateProgram(vertexShaderID, fragmentShaderID);
    free(vertexSrc);
    free(fragmentSrc);

    GPI_Shader shader;
    shader.glID = programID;

    return shader;
}

int32_t GPI_GetUniformLocation(GPI_Shader* shader, char* uniformName)
{
    int location = glGetUniformLocation(shader->glID, uniformName);
    return location;

}

void GPI_BindShader(GPI_Shader* target)
{
    glUseProgram(target->glID);
}

void GPI_UnbindShader(GPI_Shader* target)
{
    glUseProgram(0);
}
