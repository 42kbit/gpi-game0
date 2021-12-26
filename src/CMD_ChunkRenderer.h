#ifndef __HEADER_CMD_CHUNKRENDERER
#define __HEADER_CMD_CHUNKRENDERER
#include "CMD_VertexTypes.h"
#include "CMD_Meshes.h"
#include "CMD_Chunk.h"

void CMD_RegenerateChunkMesh(CMD_ChunkMesh* dst, CMD_Chunk* chunk);
void CMD_RenderChunkMesh(CMD_ChunkMesh* buffer, vec3 pos);
void CMD_SetChunkMeshBlock(CMD_Chunk* cdst, CMD_ChunkMesh* bdst, vec3 pos, CMD_BlockType* bltype);
#endif /* __HEADER_CMD_CHUNKRENDERER */
