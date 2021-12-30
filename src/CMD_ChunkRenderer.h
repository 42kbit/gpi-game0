#ifndef __HEADER_CMD_CHUNKRENDERER
#define __HEADER_CMD_CHUNKRENDERER

#include "CMD_Meshes.h"
#include "CMD_Chunk.h"
#include "CMD_BlockType.h"

#include "GPI_Camera.h"

typedef void (*CMD_BLOCKSETFUNC)(CMD_ChunkMesh*, CMD_Chunk*, vec3, CMD_BlockType*);

void CMD_RegenerateChunkMesh(CMD_ChunkMesh* dst, CMD_Chunk* chunk);
void CMD_RenderChunkMesh(CMD_ChunkMesh* buffer, vec3 pos);
void CMD_SetBlockRegenerate(CMD_ChunkMesh* mesh, CMD_Chunk* chunk, vec3 pos, CMD_BlockType* block);
void CMD_SetBlockUpdatable(CMD_ChunkMesh* mesh, CMD_Chunk* chunk, vec3 pos, CMD_BlockType* block);

void CMD_SetBlockWorldSpace(
    CMD_ChunkMesh* mesh, 
    CMD_Chunk* chunk, 
    vec2 viewportPos, 
    GPI_Camera* camera, 
    CMD_BlockType* block, 
    CMD_BLOCKSETFUNC placementFunc);
#endif /* __HEADER_CMD_CHUNKRENDERER */
