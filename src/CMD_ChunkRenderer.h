#ifndef __HEADER_CMD_CHUNKRENDERER
#define __HEADER_CMD_CHUNKRENDERER
#include "CMD_VertexTypes.h"
#include "GPI_Buffer.h"
#include "CMD_Chunk.h"

GPI_Buffer CMD_GenerateChunkMesh(CMD_Chunk* chunk);
void CMD_RenderChunkMesh(GPI_Buffer* buffer);
#endif /* __HEADER_CMD_CHUNKRENDERER */
