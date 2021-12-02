#ifndef _HEADER__GPI_TEXTURE
#define _HEADER__GPI_TEXTURE

#include <cinttypes>
#include "glad/glad.h"
struct GPI_Texture
{
    uint32_t glID;
    uint32_t textureType;
};

GPI_Texture GPI_CreateTexture(const char* path,
                      uint32_t WRAP_MODE = GL_CLAMP_TO_BORDER,
                      uint32_t FILTER_MODE = GL_NEAREST,
                      uint32_t TEXTURE_TYPE = GL_TEXTURE_2D);

void GPI_BindTexture(GPI_Texture* texture);
void GPI_UnbindTexture(GPI_Texture* texture);
#endif /* _HEADER__GPI_TEXTURE */
