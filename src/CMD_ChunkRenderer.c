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

static void CMD_ReassignVertexArray(CMD_ChunckVertex* target, uint32_t count)
{
    const CMD_ChunckVertex zerobyte; 
    memset(&zerobyte, 0, sizeof(zerobyte));
    for(uint32_t i = 0; i < count; i++)
    {
        if(memcmp(target+i, &zerobyte, sizeof(CMD_ChunckVertex)) == 0)
            for(uint32_t j = 0; j < count - i; j++)
                if(memcmp(target+i+j, &zerobyte, sizeof(CMD_ChunckVertex)) != 0)
                {
                    memcpy(target+i, target+i+j, sizeof(CMD_ChunckVertex));
                    memset(target+i+j, 0, sizeof(CMD_ChunckVertex));
                    break;
                }
            
    }
}

static vec3 opx = {1,0,0};
static vec3 onx = {-1,0,0};
static vec3 opy = {0,1,0};
static vec3 ony = {0,-1,0};
static vec3 opz = {0,0,1};
static vec3 onz = {0,0,-1}; 
static vec3* direcions[6] = {opx, onx, opy, ony, opz, onz};

static void CMD_UpdateBufferBlock(CMD_ChunkMesh* buffer, CMD_Chunk* c, vec3 pos)
{
    CMD_ChunckVertex nearSides[4];
    uint8_t nearSidesTop = 0;
    memset(nearSides, 0, sizeof(nearSides));
    for(uint32_t i = 0; i < 6; i++){
        if(!CMD_DrawSide(c, pos, direcions[i]))
        {
            for(uint32_t j = 0; j < 4; j++){
            nearSides[nearSidesTop++] = CMD_MapChunkVertexData(
            CMD_VOXEL_VERTECIES[j*4+0+i*16] +(uint32_t)pos[0],
            CMD_VOXEL_VERTECIES[j*4+1+i*16] +(uint32_t)pos[1],
            CMD_VOXEL_VERTECIES[j*4+2+i*16] +(uint32_t)pos[2],
            CMD_VOXEL_VERTECIES[j*4+3+i*16], 
            1
            );
            }

            uint32_t quadCnt = (buffer->verticesTop) / 4;
            CMD_ChunckVertex* quad = nearSides;

            nearSidesTop = 0;
            uint8_t vertexCnted = 0; 
            CMD_ChunckVertex* bufferVertex;
            for(uint32_t k = 0; k < quadCnt; k++)
            {
                for(uint32_t q = 0; q < 4; q++){
                    bufferVertex = buffer->vertices + k*4 + q;
                    uint32_t qx = CMD_UNMAP_CHUNK_POSX(quad[q].data0);
                    uint32_t qy = CMD_UNMAP_CHUNK_POSY(quad[q].data0);
                    uint32_t qz = CMD_UNMAP_CHUNK_POSZ(quad[q].data0);
                    if(qx == CMD_UNMAP_CHUNK_POSX(bufferVertex->data0) && 
                       qy == CMD_UNMAP_CHUNK_POSY(bufferVertex->data0) &&
                       qz == CMD_UNMAP_CHUNK_POSZ(bufferVertex->data0))
                    {
                        vertexCnted++;
                    }
                    if(vertexCnted == 4)
                    {
                        memset(bufferVertex-3, 0, sizeof(CMD_ChunckVertex)*4);
                        CMD_ReassignVertexArray(buffer->vertices, buffer->verticesTop);
                        buffer->verticesTop -= 4;
                        goto loopEND;
                    }
                }
                vertexCnted = 0;
            }
            
            loopEND:;
            
        }
    }
}

static void CMD_SetBufferBlock(CMD_ChunkMesh* buffer, CMD_Chunk* c, vec3 pos)
{
    CMD_ChunckVertex data[24];
    uint32_t dataTop = 0;
    memset(data, 0, sizeof(data));
    uint32_t oldTop = buffer->verticesTop;
    for(uint32_t i = 0; i < 6; i++){
        if(CMD_DrawSide(c, pos, direcions[i]))
        {
            for(uint32_t j = 0; j < 4; j++){
            data[dataTop++] = CMD_MapChunkVertexData(
            CMD_VOXEL_VERTECIES[j*4+0+i*4*4] +(uint32_t)pos[0],
            CMD_VOXEL_VERTECIES[j*4+1+i*4*4] +(uint32_t)pos[1],
            CMD_VOXEL_VERTECIES[j*4+2+i*4*4] +(uint32_t)pos[2],
            CMD_VOXEL_VERTECIES[j*4+3+i*4*4], 
            1
            );
            }
            buffer->iboTop+=6;
        }
    }
    memcpy(buffer->vertices + buffer->verticesTop, data, dataTop*sizeof(CMD_ChunckVertex));
    buffer->verticesTop += dataTop;
}

void CMD_SetChunkMeshBlock(CMD_Chunk* cdst, CMD_ChunkMesh* bdst, vec3 pos, CMD_BlockType* bltype)
{
    uint32_t index = CMD_GetParrayOffset(pos);
    if(!CMD_IsInChunk(pos)) return;
    if(cdst->blocks[index] != &CMD_AirBlock)
        return;
    cdst->blocks[index] = bltype;
    CMD_SetBufferBlock(bdst, cdst, pos);
    for(uint32_t i = 0; i < 6; i++)
    {
        vec3 dst;
        glm_vec3_add(pos, direcions[i], dst);
        if(CMD_IsInChunk(dst) && !cdst->blocks[CMD_GetParrayOffset(dst)]->isTransparent)
            CMD_UpdateBufferBlock(bdst, cdst, dst);
    }
    CMD_PushChunkVBOData(bdst);
}

void CMD_RegenerateChunkMesh(CMD_ChunkMesh* dst, CMD_Chunk* chunk)
{
    for(uint32_t x = 0; x < CMD_CHUNK_COUNT_X; x++)
    for(uint32_t y = 0; y < CMD_CHUNK_COUNT_Y; y++)
    for(uint32_t z = 0; z < CMD_CHUNK_COUNT_Z; z++)
    {
        uint32_t offset = (x*CMD_CHUNK_COUNT_Y*CMD_CHUNK_COUNT_Z + y*CMD_CHUNK_COUNT_Z +z)*24;
        vec3 currentPos = {x,y,z};
        for(uint32_t i = 0; i < 6; i++){
            if(CMD_DrawSide(chunk, currentPos, direcions[i]))
            {
                for(uint32_t j = 0; j < 4; j++){
                dst->vertices[dst->verticesTop] = CMD_MapChunkVertexData(
                CMD_VOXEL_VERTECIES[j*4+0+i*4*4] +x,
                CMD_VOXEL_VERTECIES[j*4+1+i*4*4] +y,
                CMD_VOXEL_VERTECIES[j*4+2+i*4*4] +z,
                CMD_VOXEL_VERTECIES[j*4+3+i*4*4], 
                2
                );
                dst->verticesTop++;
                }
                dst->iboTop+=6;
            }
        }
    }
    dst->ibo = CMD_ChunkIBO;
    GPI_BindBuffer(&dst->vbo);
        glBufferSubData(dst->vbo.TYPE, 0, CMD_CHUNK_COUNT_ALL * 24 * sizeof(CMD_ChunckVertex), dst->vertices);
}

void CMD_RenderChunkMesh(CMD_ChunkMesh* mesh, vec3 pos)
{
    GPI_BindVertexArray(&mesh->vao);
    GPI_BindVertexArrayAttribs(&mesh->vao);
    GPI_BindBuffer(&mesh->vbo);
    GPI_BindBuffer(&mesh->ibo);

    GPI_BindShader(&CMD_ChunkShader);
    int32_t loc = GPI_GetUniformLocation(&CMD_ChunkShader, "u_ChunkPosition");
    if(loc != -1)
        glUniform3f(loc, pos[0], pos[1], pos[2]);
    glDrawElements(
        GL_TRIANGLES,
        mesh->iboTop, 
        GL_UNSIGNED_INT, 
        NULL);
}