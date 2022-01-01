#include "CMD_ChunkData.h"
#include <malloc.h>
#include <string.h>

CMD_ChunkData CMD_CreateChunkData(uint32_t pos[3])
{
    CMD_ChunkData c;
    c.blocks = malloc(CMD_CHUNK_COUNT_ALL * sizeof(*c.blocks));
    memset(c.blocks, 0, CMD_CHUNK_COUNT_ALL * sizeof(*c.blocks));
    memcpy(c.position, pos, sizeof(uint32_t)*3);
    return c;
}

CMD_ChunkData CMD_AllocateChunkData()
{
    CMD_ChunkData c;
    c.blocks = malloc(CMD_CHUNK_COUNT_ALL * sizeof(*c.blocks));
    memset(c.blocks, 0, CMD_CHUNK_COUNT_ALL * sizeof(*c.blocks));
    return c;
}

void CMD_FreeChunkData(CMD_ChunkData* chunk)
{
    free(chunk->blocks);
}
uint8_t CMD_IsInChunk(vec3 pos)
{
    return 
    (pos[0] >= 0) && (pos[0] < CMD_CHUNK_COUNT_X) &&
    (pos[1] >= 0) && (pos[1] < CMD_CHUNK_COUNT_Y) &&
    (pos[2] >= 0) && (pos[2] < CMD_CHUNK_COUNT_Z);
}

uint32_t CMD_GetParrayOffset(vec3 p)
{
    return 
    (uint32_t)p[0] * CMD_CHUNK_COUNT_Y * CMD_CHUNK_COUNT_Z + 
    (uint32_t)p[1] * CMD_CHUNK_COUNT_Z + 
    (uint32_t)p[2];
}

uint8_t CMD_IsInChunkOffset(vec3 pos, vec3 offset)
{  
    return 
    (pos[0] + offset[0] >= 0) && (pos[0] + offset[0] < CMD_CHUNK_COUNT_X) &&
    (pos[1] + offset[1] >= 0) && (pos[1] + offset[1] < CMD_CHUNK_COUNT_Y) &&
    (pos[2] + offset[2] >= 0) && (pos[2] + offset[2] < CMD_CHUNK_COUNT_Z);
}

void CMD_SetBlockChunk(CMD_ChunkData* c, vec3 pos, CMD_BlockType* type){
    if(!CMD_IsInChunk(pos)) return;
    c->blocks[CMD_GetParrayOffset(pos)] = type;
}