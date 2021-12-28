#ifndef __HEADER_CMD_CHUNKRENDERER
#define __HEADER_CMD_CHUNKRENDERER

#include "CMD_Meshes.h"
#include "CMD_Chunk.h"
#include "CMD_BlockType.h"

void CMD_RegenerateChunkMesh(CMD_ChunkMesh* dst, CMD_Chunk* chunk);
void CMD_RenderChunkMesh(CMD_ChunkMesh* buffer, vec3 pos);
#endif /* __HEADER_CMD_CHUNKRENDERER */
