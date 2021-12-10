#ifndef _HEADER__GPI_TEXTUREREADER
#define _HEADER__GPI_TEXTUREREADER

#include <inttypes.h>


uint32_t GPI_ReadTexture(const char* path,
                     uint32_t WRAP_MODE,
                     uint32_t FILTER_MODE,
                     uint32_t TEXTURE_TYPE);

#endif /* _HEADER__GPI_TEXTUREREADER */
