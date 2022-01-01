#ifndef __HEADER_CMD_WORLD
#define __HEADER_CMD_WORLD

#include "CMD_ChunkData.h"
#include "CMD_Meshes.h"

#include "vec3.h"

typedef struct 
{
    CMD_ChunkData chunkData;
    CMD_ChunkMesh chunkMesh;
} CMD_Chunk;

typedef struct {
    CMD_Chunk* loadedChunks;
    uint32_t loadedChunksTop;
} CMD_World;

void CMD_CreateWorld(CMD_World* dst);
void CMD_AllocateEmptyChunk(CMD_World* world, CMD_Chunk** dst);
void CMD_PushChunkData(CMD_Chunk* dst, CMD_ChunkData* newData);
void CMD_RenderAllChunks(CMD_World* world);

#endif /* __HEADER_CMD_WORLD */
