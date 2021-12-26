#ifndef __HEADER_CMD_MESHES
#define __HEADER_CMD_MESHES

#include "GPI_Buffer.h"
#include "GPI_VertexArray.h"

#include "CMD_VertexTypes.h"

typedef struct 
{
    GPI_VertexArray vao;
    GPI_Buffer vbo;
    GPI_Buffer ibo;
    uint32_t iboTop;

    CMD_ChunckVertex* vertices;
    uint32_t verticesTop;
} CMD_ChunkMesh;
CMD_ChunkMesh CMD_CreateMesh(
    uint32_t VBOTYPE, uint32_t VBOsize, uint32_t VBODRAWMODE, 
    uint32_t IBOTYPE, uint32_t IBOsize, uint32_t IBODRAWMODE, 
    GPI_VertexLayout* layout);
void CMD_FreeChunkMesh(CMD_ChunkMesh* mesh);
void CMD_PushChunkVBOData(CMD_ChunkMesh* target);
#endif /* __HEADER_CMD_MESHES */
