#ifndef _HEADER__GPI_TEXTURE
#define _HEADER__GPI_TEXTURE

#include <inttypes.h>
#include "glad/glad.h"
typedef struct 
{
    uint32_t glID;
    uint32_t textureType;
} GPI_Texture;

GPI_Texture GPI_CreateTexture(const char* path,
                      uint32_t WRAP_MODE,
                      uint32_t FILTER_MODE,
                      uint32_t TEXTURE_TYPE);

void GPI_BindTexture(GPI_Texture* texture);
void GPI_UnbindTexture(GPI_Texture* texture);
#endif /* _HEADER__GPI_TEXTURE */
