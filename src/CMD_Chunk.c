#include "CMD_Chunk.h"
#include <malloc.h>
#include <string.h>

CMD_Chunk CMD_CreateChunk(vec3 pos)
{
    CMD_Chunk c;
    c.blocks = malloc(CMD_CHUNK_COUNT_ALL * sizeof(*c.blocks));
    memset(c.blocks, 0, CMD_CHUNK_COUNT_ALL * sizeof(*c.blocks));
    memcpy(c.position, pos, sizeof(vec3));
    return c;
}

void CMD_FreeChunk(CMD_Chunk* chunk)
{
    free(chunk->blocks);
}