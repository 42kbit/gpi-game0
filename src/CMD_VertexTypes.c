#include "CMD_VertexTypes.h"

GPI_VertexLayout CMD_GetDefaultVertexLayout()
{
    GPI_VertexLayout result = GPI_CreateVertexLayout();
    GPI_PushLayoutData(&result, 3, GL_FLOAT, GL_FALSE);
    GPI_PushLayoutData(&result, 2, GL_FLOAT, GL_FALSE);
    GPI_PushLayoutData(&result, 1, GL_FLOAT, GL_FALSE);
    return result;
}

GPI_VertexLayout CMD_GetChunkVertexLayout()
{
    GPI_VertexLayout result = GPI_CreateVertexLayout();
    GPI_PushLayoutData(&result, 1, GL_UNSIGNED_INT, GL_FALSE);
    GPI_PushLayoutData(&result, 1, GL_UNSIGNED_INT, GL_FALSE);
    return result;
}

CMD_ChunckVertex CMD_MapChunkVertexData( // chunksize = 16x16x256
    uint8_t posX, // pos is relative to chunckPos 
    uint8_t posY, 
    uint8_t posZ,
    uint8_t textureKey, // 0 - 3 key (which part of quad is this)
    uint32_t textureIndex // index on 16*16 texture atlas
)
{
    uint32_t data0 = 0;
    data0 = posX << 28 | posY << 20 | posZ << 16 | textureKey << 14 | textureIndex << 6;
    CMD_ChunckVertex v = {data0, 0};
    return v;   
}