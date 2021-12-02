#include "GPI_Shader.h"

#include "GPI_FileReader.h"
#include "GPI_ShaderCompiler.h"

#include "glad/glad.h"

GPI_Shader GPI_CreateShaderFromFiles(const char* vertexPath,
                                             const char* fragmentPath)
{
    std::string vertexSrc = GPI_ReadFile(std::string(vertexPath));
    std::string fragmentSrc = GPI_ReadFile(std::string(fragmentPath));

    uint32_t vertexShaderID = GPI_CreateShader(vertexSrc.c_str(), GL_VERTEX_SHADER);
    uint32_t fragmentShaderID = GPI_CreateShader(fragmentSrc.c_str(), GL_FRAGMENT_SHADER);

    uint32_t programID = GPI_CreateProgram(vertexShaderID, fragmentShaderID);
    return {programID};
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
