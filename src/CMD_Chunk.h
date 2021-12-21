#ifndef __HEADER_CMD_CHUNK
#define __HEADER_CMD_CHUNK
#include "CMD_BlockType.h"
#include "CMD_VertexTypes.h"
#include "vec3.h"

#define CMD_CHUNK_COUNT_X 16
#define CMD_CHUNK_COUNT_Y 256
#define CMD_CHUNK_COUNT_Z 16

#define CMD_CHUNK_COUNT_ALL CMD_CHUNK_COUNT_X*CMD_CHUNK_COUNT_Y*CMD_CHUNK_COUNT_Z

typedef struct 
{
    vec3 position;
    CMD_BlockType** blocks;
} CMD_Chunk;
CMD_Chunk CMD_CreateChunk(vec3 pos);
void CMD_FreeChunk(CMD_Chunk* chunk);

#endif /* __HEADER_CMD_CHUNK */
