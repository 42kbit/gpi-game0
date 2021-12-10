#include "GPI_Texture.h"
#include "GPI_TextureReader.h"

GPI_Texture GPI_CreateTexture(const char* path,
                                      uint32_t WRAP_MODE,
                                      uint32_t FILTER_MODE,
                                      uint32_t TEXTURE_TYPE){
    uint32_t glID = GPI_ReadTexture(path, WRAP_MODE, FILTER_MODE, TEXTURE_TYPE);
    GPI_Texture texture = {glID, TEXTURE_TYPE};
    return texture;
}

void GPI_BindTexture(GPI_Texture* target)
{
    glBindTexture(target->textureType, target->glID);
}

void GPI_UnbindTexture(GPI_Texture* target)
{
    glBindTexture(target->textureType, 0);
}
