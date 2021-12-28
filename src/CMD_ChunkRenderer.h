#ifndef __HEADER_CMD_CHUNKRENDERER
#define __HEADER_CMD_CHUNKRENDERER

#include "CMD_Meshes.h"
#include "CMD_Chunk.h"
#include "CMD_BlockType.h"

void CMD_RegenerateChunkMesh(CMD_ChunkMesh* dst, CMD_Chunk* chunk);
void CMD_RenderChunkMesh(CMD_ChunkMesh* buffer, vec3 pos);
void CMD_SetBlockRegenerate(CMD_ChunkMesh* mesh, CMD_Chunk* chunk, vec3 pos, CMD_BlockType* block);
#endif /* __HEADER_CMD_CHUNKRENDERER */
