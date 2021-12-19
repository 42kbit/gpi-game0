#include "CMD_ChunkRenderer.h"
#include "CMD_Global.h"

GPI_Buffer CMD_GenerateChunkMesh(CMD_Chunk* chunk)
{
    CMD_ChunckVertex* vertecies = (CMD_ChunckVertex*)malloc(
        CMD_CHUNK_COUNT_ALL * 24 * sizeof(CMD_ChunckVertex));
    for(uint32_t x = 0; x < CMD_CHUNK_COUNT_X; x++)
    for(uint32_t y = 0; y < CMD_CHUNK_COUNT_Y; y++)
    for(uint32_t z = 0; z < CMD_CHUNK_COUNT_Z; z++)
    {
        uint32_t offset = (x*CMD_CHUNK_COUNT_Y*CMD_CHUNK_COUNT_Z + y*CMD_CHUNK_COUNT_Z +z)*24;
        for(uint32_t j = 0; j < 24; j++){
            vertecies[offset+j] = CMD_MapChunkVertexData(
            CMD_VOXEL_VERTECIES[j*4+0] +x,
            CMD_VOXEL_VERTECIES[j*4+1] +y,
            CMD_VOXEL_VERTECIES[j*4+2] +z,
            CMD_VOXEL_VERTECIES[j*4+3], 
            0 // TODO: Change the id 
            );
        }
    }
    GPI_Buffer b = GPI_CreateBuffer(
        GL_ARRAY_BUFFER, 
        CMD_CHUNK_COUNT_ALL * 24 * sizeof(CMD_ChunckVertex),
        vertecies,
        GL_STATIC_DRAW
    );
    free(vertecies);
    return b;
}