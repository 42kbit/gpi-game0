#ifndef __HEADER_CMD_CHUNKDATA
#define __HEADER_CMD_CHUNKDATA

#include "CMD_BlockType.h"
#include "CMD_VertexTypes.h"
#include "vec3.h"

#define CMD_CHUNK_COUNT_X 16
#define CMD_CHUNK_COUNT_Y 256
#define CMD_CHUNK_COUNT_Z 16

#define CMD_CHUNK_COUNT_ALL CMD_CHUNK_COUNT_X*CMD_CHUNK_COUNT_Y*CMD_CHUNK_COUNT_Z

typedef struct 
{
    uint32_t position[3];
    CMD_BlockType** blocks;
} CMD_ChunkData;
CMD_ChunkData CMD_CreateChunkData(uint32_t pos[3]);
CMD_ChunkData CMD_AllocateChunkData();
void CMD_FreeChunkData(CMD_ChunkData* chunk);
uint32_t CMD_GetParrayOffset(vec3 p);
uint8_t CMD_IsInChunk(vec3 pos);
uint8_t CMD_IsInChunkOffset(vec3 pos, vec3 offset);
void CMD_SetBlockChunk(CMD_ChunkData* c, vec3 pos, CMD_BlockType* type);

#endif /* __HEADER_CMD_CHUNKDATA */
