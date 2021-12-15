#ifndef __HEADER_CMD_VERTEXTYPES
#define __HEADER_CMD_VERTEXTYPES

#include "struct/vec2.h"
#include "struct/vec3.h"

#include "GPI_VertexLayout.h"

typedef struct 
{
    vec3 position;
    vec2 textureCoords;
    float texIndex;
} CMD_VertexDefault;
GPI_VertexLayout CMD_GetDefaultVertexLayout();

typedef struct
{
    uint32_t data0;
    uint32_t data1;
} CMD_ChunckVertex;
CMD_ChunckVertex CMD_MapChunkVertexData(
    uint16_t posX, // pos is relative to chunckPos 
    uint16_t posY, 
    uint16_t posZ,
    uint8_t textureKey, // 0 - 3 key (which part of quad is this)
    uint8_t textureIndex // index on 16*16 texture atlas
    );  
GPI_VertexLayout CMD_GetChunkVertexLayout();
#endif /* __HEADER_CMD_VERTEXTYPES */
