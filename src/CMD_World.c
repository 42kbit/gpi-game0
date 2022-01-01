#include "CMD_World.h"
#include "CMD_Global.h"

void CMD_CreateWorld(CMD_World* dst)
{
    dst->loadedChunksTop = 0;
    dst->loadedChunks = (CMD_Chunk*)malloc(CMD_CHUNK_RENDER_AREA * sizeof(*dst->loadedChunks));
    memset(dst->loadedChunks, 0, CMD_CHUNK_RENDER_AREA * sizeof(*dst->loadedChunks));
}

void CMD_AllocateEmptyChunk(CMD_World* world, CMD_Chunk** dst)
{
    *dst = world->loadedChunks + world->loadedChunksTop;
    world->loadedChunks[world->loadedChunksTop++].chunkData = CMD_AllocateChunkData();
    (*dst)->chunkMesh = CMD_CreateMesh(
        GL_ARRAY_BUFFER, CMD_CHUNK_COUNT_ALL*24*sizeof(CMD_ChunkVertex), GL_DYNAMIC_DRAW,
        GL_ELEMENT_ARRAY_BUFFER, CMD_CHUNK_COUNT_ALL*36*sizeof(uint32_t), GL_DYNAMIC_DRAW, 
        &CMD_ChunkLayout);
}

void CMD_PushChunkData(CMD_Chunk* dst, CMD_ChunkData* newData)
{
    memcpy(&dst->chunkData, newData, sizeof(CMD_ChunkData));
    CMD_RegenerateChunkMesh(&dst->chunkMesh, &dst->chunkData);
}

void CMD_RenderAllChunks(CMD_World* world)
{
    for(uint32_t i = 0; i < world->loadedChunksTop; i++)
        CMD_RenderChunkMesh(&world->loadedChunks[i].chunkMesh, &world->loadedChunks[i].chunkData);
}

