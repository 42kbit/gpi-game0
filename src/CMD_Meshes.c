#include "CMD_Meshes.h"
#include <malloc.h>
#include <string.h>

CMD_ChunkMesh CMD_CreateMesh(
    uint32_t VBOTYPE, uint32_t VBOsize, uint32_t VBODRAWMODE, 
    uint32_t IBOTYPE, uint32_t IBOsize, uint32_t IBODRAWMODE,
    GPI_VertexLayout* layout)
{
    CMD_ChunkMesh m;
    m.vbo = GPI_CreateBuffer(VBOTYPE, VBOsize, NULL, VBODRAWMODE);
    m.ibo = GPI_CreateBuffer(IBOTYPE, IBOsize, NULL, IBODRAWMODE);
    m.iboTop = 0;
    m.vao = GPI_CreateVertexArray(layout, &m.vbo, &m.ibo);
    m.vertices = (CMD_ChunkVertex*)malloc(VBOsize);
    memset(m.vertices, 0, VBOsize);
    m.verticesTop = 0;
    return m;
}

void CMD_FreeChunkMesh(CMD_ChunkMesh* dst)
{
    glDeleteBuffers(1, &dst->vbo.glID);
    free(dst->vertices);
}

void CMD_PushChunkVBOData(CMD_ChunkMesh* target)
{
    glNamedBufferSubData(target->vbo.glID, 0, (target->verticesTop)*sizeof(CMD_ChunkVertex), target->vertices);
}