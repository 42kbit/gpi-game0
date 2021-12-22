#include "CMD_ChunkRenderer.h"
#include "CMD_Global.h"
#include "GPI_VertexArray.h"

#include <string.h>

static uint8_t CMD_DrawSide(CMD_Chunk* c, vec3 pos, vec3 offset)
{
    if(!CMD_IsInChunkOffset(pos, offset)) return c->blocks[CMD_GetParrayOffset(pos)]->isTransparent?  0 : 1;
    uint32_t currentBlockpos = (uint32_t)(pos[0])*CMD_CHUNK_COUNT_Y*CMD_CHUNK_COUNT_Z + (uint32_t)(pos[1])*CMD_CHUNK_COUNT_Z +(uint32_t)(pos[2]);
    uint32_t arraypos = (uint32_t)(pos[0]+offset[0])*CMD_CHUNK_COUNT_Y*CMD_CHUNK_COUNT_Z + (uint32_t)(pos[1]+offset[1])*CMD_CHUNK_COUNT_Z +(uint32_t)(pos[2]+offset[2]);
    return !c->blocks[currentBlockpos]->isTransparent && c->blocks[arraypos]->isTransparent;
}

static void CMD_ChangeBufferBlock(GPI_Buffer* buffer, CMD_Chunk* c, vec3 pos)
{
    uint32_t index = CMD_GetParrayOffset(pos);
    CMD_ChunckVertex data[24];
    memset(data, 0, sizeof(data));
    vec3 opx = {1,0,0};
    vec3 onx = {-1,0,0};
    vec3 opy = {0,1,0};
    vec3 ony = {0,-1,0};
    vec3 opz = {0,0,1};
    vec3 onz = {0,0,-1}; 
    vec3* direcions[6] = {opx, onx, opy, ony, opz, onz};
    for(uint32_t i = 0; i < 6; i++){
        if(CMD_DrawSide(c, pos, direcions[i]))
        {
            for(uint32_t j = 0; j < 4; j++){
            data[j+i*4] = CMD_MapChunkVertexData(
            CMD_VOXEL_VERTECIES[j*4+0+i*4*4] +(uint32_t)pos[0],
            CMD_VOXEL_VERTECIES[j*4+1+i*4*4] +(uint32_t)pos[1],
            CMD_VOXEL_VERTECIES[j*4+2+i*4*4] +(uint32_t)pos[2],
            CMD_VOXEL_VERTECIES[j*4+3+i*4*4], 
            1
            );}
        } // TODO: recalculate neighboards
    }
    GPI_BindBuffer(buffer);
    glBufferSubData(buffer->TYPE, index*sizeof(CMD_ChunckVertex)*24, sizeof(CMD_ChunckVertex)*24, data);
}

void CMD_RegenerateChunkMesh(GPI_Buffer* dst, CMD_Chunk* chunk)
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
            if(CMD_DrawSide(chunk, currentPos, direcions[i]))
            {
                for(uint32_t j = 0; j < 4; j++){
                vertecies[offset+j+i*4] = CMD_MapChunkVertexData(
                CMD_VOXEL_VERTECIES[j*4+0+i*4*4] +x,
                CMD_VOXEL_VERTECIES[j*4+1+i*4*4] +y,
                CMD_VOXEL_VERTECIES[j*4+2+i*4*4] +z,
                CMD_VOXEL_VERTECIES[j*4+3+i*4*4], 
                2
                );}
            }
        }
        
    }
    GPI_BindBuffer(dst);
        glBufferSubData(dst->TYPE, 0, CMD_CHUNK_COUNT_ALL * 24 * sizeof(CMD_ChunckVertex), vertecies);
    free(vertecies);
}

void CMD_SetChunkMeshBlock(CMD_Chunk* cdst, GPI_Buffer* bdst, vec3 pos, CMD_BlockType* bltype)
{
    uint32_t index = CMD_GetParrayOffset(pos);
    if(!CMD_IsInChunk(pos)) return;
    cdst->blocks[index] = bltype;
    vec3 opx = {1,0,0};
    vec3 onx = {-1,0,0};
    vec3 opy = {0,1,0};
    vec3 ony = {0,-1,0};
    vec3 opz = {0,0,1};
    vec3 onz = {0,0,-1}; 
    vec3* direcions[6] = {opx, onx, opy, ony, opz, onz};
    CMD_ChangeBufferBlock(bdst, cdst, pos);
    for(uint32_t i = 0; i < 6; i++)
    {
        vec3 dst;
        glm_vec3_add(pos, direcions[i], dst);
        CMD_ChangeBufferBlock(bdst, cdst, dst);
    }
}

void CMD_RenderChunkMesh(GPI_Buffer* buffer)
{
    GPI_VertexArray vao = GPI_CreateVertexArray(&CMD_ChunkLayout, buffer, &CMD_ChunkIBO);
    GPI_BindVertexArray(&vao);
    GPI_BindVertexArrayAttribs(&vao);
    GPI_BindShader(&CMD_ChunkShader);
    glDrawElements(GL_TRIANGLES, 36*CMD_CHUNK_COUNT_ALL, GL_UNSIGNED_INT, NULL);
}