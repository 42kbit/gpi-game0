#ifndef _HEADER__GPI_SHADER
#define _HEADER__GPI_SHADER

#include <cinttypes>

struct GPI_Shader
{
    uint32_t glID;
};

GPI_Shader GPI_CreateShaderFromFiles(const char* vertexPath,
                             const char* fragmentPath);

int32_t GPI_GetUniformLocation(GPI_Shader* shader, char* uniformName);

void GPI_BindShader(GPI_Shader* target);
void GPI_UnbindShader(GPI_Shader* target);

#endif /* _HEADER__GPI_SHADER */
