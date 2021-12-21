#include "CMD_ChunkRenderer.h"
#include "CMD_Global.h"

#include <string.h>

static uint8_t CMD_IsInChunk(vec3 pos, vec3 offset)
{  
    return 
    (pos[0] + offset[0] >= 0) && (pos[0] + offset[0] < CMD_CHUNK_COUNT_X) &&
    (pos[1] + offset[1] >= 0) && (pos[1] + offset[1] < CMD_CHUNK_COUNT_Y) &&
    (pos[2] + offset[2] >= 0) && (pos[2] + offset[2] < CMD_CHUNK_COUNT_Z);
}

static uint8_t CMD_IsTransparentAt(CMD_Chunk* c, vec3 pos, vec3 offset)
{
    uint32_t arraypos = (uint32_t)(pos[0]+offset[0])*CMD_CHUNK_COUNT_Y*CMD_CHUNK_COUNT_Z + (uint32_t)(pos[1]+offset[1])*CMD_CHUNK_COUNT_Z +(uint32_t)(pos[2]+offset[2]);
    return !CMD_IsInChunk(pos, offset) || (c->blocks[arraypos]->isTransparent);
    // return CMD_IsInChunk(pos, offset) && (c->blocks[arraypos]->isTransparent); cull chunk faces
}

GPI_Buffer CMD_GenerateChunkMesh(CMD_Chunk* chunk)
{
    CMD_ChunckVertex* vertecies = (CMD_ChunckVertex*)malloc(
        CMD_CHUNK_COUNT_ALL * 24 * sizeof(CMD_ChunckVertex)); // max size
    memset(vertecies, 0, CMD_CHUNK_COUNT_ALL * 24 * sizeof(CMD_ChunckVertex));
    for(uint32_t x = 0; x < CMD_CHUNK_COUNT_X; x++)
    for(uint32_t y = 0; y < CMD_CHUNK_COUNT_Y; y++)
    for(uint32_t z = 0; z < CMD_CHUNK_COUNT_Z; z++)
    {
        uint32_t offset = (x*CMD_CHUNK_COUNT_Y*CMD_CHUNK_COUNT_Z + y*CMD_CHUNK_COUNT_Z +z)*24;

        vec3 currentPos = {x,y,z};
        vec3 opx = {1,0,0};
        vec3 onx = {-1,0,0};
        vec3 opy = {0,1,0};
        vec3 ony = {0,-1,0};
        vec3 opz = {0,0,1};
        vec3 onz = {0,0,-1}; 
        vec3* direcions[6] = {opx, onx, opy, ony, opz, onz};
        for(uint32_t i = 0; i < 6; i++){
            if(CMD_IsTransparentAt(chunk, currentPos, direcions[i]))
            {
                for(uint32_t j = 0; j < 4; j++){
                vertecies[offset+j+i*4] = CMD_MapChunkVertexData(
                CMD_VOXEL_VERTECIES[j*4+0+i*4*4] +x,
                CMD_VOXEL_VERTECIES[j*4+1+i*4*4] +y,
                CMD_VOXEL_VERTECIES[j*4+2+i*4*4] +z,
                CMD_VOXEL_VERTECIES[j*4+3+i*4*4], 
                0 // TODO: Change the id 
                );}
            }
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