#ifndef __HEADER_CMD_CHUNKRENDERER
#define __HEADER_CMD_CHUNKRENDERER
#include "CMD_VertexTypes.h"
#include "GPI_Buffer.h"
#include "CMD_Chunk.h"

void CMD_RegenerateChunkMesh(GPI_Buffer* dst, CMD_Chunk* chunk);
void CMD_RenderChunkMesh(GPI_Buffer* buffer);
void CMD_SetChunkMeshBlock(CMD_Chunk* cdst, GPI_Buffer* bdst, vec3 pos, CMD_BlockType* bltype);
#endif /* __HEADER_CMD_CHUNKRENDERER */
