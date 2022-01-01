#ifndef __HEADER_CMD_CHUNKRENDERER
#define __HEADER_CMD_CHUNKRENDERER

#include "CMD_Meshes.h"
#include "CMD_ChunkData.h"
#include "CMD_BlockType.h"

#include "GPI_Camera.h"

typedef void (*CMD_BLOCKSETFUNC)(CMD_ChunkMesh*, CMD_ChunkData*, vec3, CMD_BlockType*);

void CMD_RegenerateChunkMesh(CMD_ChunkMesh* dst, CMD_ChunkData* chunk);
void CMD_RenderChunkMesh(CMD_ChunkMesh* buffer, CMD_ChunkData* data);
void CMD_SetBlockRegenerate(CMD_ChunkMesh* mesh, CMD_ChunkData* chunk, vec3 pos, CMD_BlockType* block);
void CMD_SetBlockUpdatable(CMD_ChunkMesh* mesh, CMD_ChunkData* chunk, vec3 pos, CMD_BlockType* block);

void CMD_SetBlockWorldSpace(
    CMD_ChunkMesh* mesh, 
    CMD_ChunkData* chunk, 
    vec2 viewportPos, 
    GPI_Camera* camera, 
    CMD_BlockType* block, 
    CMD_BLOCKSETFUNC placementFunc);
#endif /* __HEADER_CMD_CHUNKRENDERER */
