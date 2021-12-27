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
} CMD_ChunkVertex;
GPI_VertexLayout CMD_GetChunkVertexLayout();
CMD_ChunkVertex CMD_MapChunkVertexData(
    uint8_t posX, // pos is relative to chunckPos 
    uint16_t posY, 
    uint8_t posZ,
    uint8_t textureKey, // 0 - 3 key (which part of quad is this)
    uint8_t textureIndex // index on 16*16 texture atlas
);  
#define CMD_UNMAP_CHUNK_POSX(v) (v & 0xF8000000) >> 27
#define CMD_UNMAP_CHUNK_POSY(v) (v & 0x07FC0000) >> 18
#define CMD_UNMAP_CHUNK_POSZ(v) (v & 0x0003E000) >> 13

#endif /* __HEADER_CMD_VERTEXTYPES */
