#include "CMD_ChunkRenderer.h"
#include "CMD_Global.h"
#include "GPI_VertexArray.h"


#include <string.h>

static uint8_t CMD_DrawSide(CMD_Chunk* c, vec3 pos, vec3 offset)
{
    if(!CMD_IsInChunkOffset(pos, offset)) return c->blocks[CMD_GetParrayOffset(pos)]->flags?  0 : 1;
    uint32_t currentBlockpos = (uint32_t)(pos[0])*CMD_CHUNK_COUNT_Y*CMD_CHUNK_COUNT_Z + (uint32_t)(pos[1])*CMD_CHUNK_COUNT_Z +(uint32_t)(pos[2]);
    uint32_t arraypos = (uint32_t)(pos[0]+offset[0])*CMD_CHUNK_COUNT_Y*CMD_CHUNK_COUNT_Z + (uint32_t)(pos[1]+offset[1])*CMD_CHUNK_COUNT_Z +(uint32_t)(pos[2]+offset[2]);
    return !(c->blocks[currentBlockpos]->flags & CMD_BLOCK_TRANSPARENCY_MASK) && (c->blocks[arraypos]->flags & CMD_BLOCK_TRANSPARENCY_MASK);
}

static void CMD_ReassignVertexArray(CMD_ChunkVertex* target, uint32_t count)
{
    const CMD_ChunkVertex zerobyte; 
    memset(&zerobyte, 0, sizeof(zerobyte));
    for(uint32_t i = 0; i < count; i++)
    {
        if(memcmp(target+i, &zerobyte, sizeof(CMD_ChunkVertex)) == 0)
            for(uint32_t j = 0; j < count - i; j++)
                if(memcmp(target+i+j, &zerobyte, sizeof(CMD_ChunkVertex)) != 0)
                {
                    memcpy(target+i, target+i+j, sizeof(CMD_ChunkVertex));
                    memset(target+i+j, 0, sizeof(CMD_ChunkVertex));
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

void CMD_RegenerateChunkMesh(CMD_ChunkMesh* dst, CMD_Chunk* chunk)
{
    dst->iboTop = 0;
    dst->verticesTop = 0;
    for(uint32_t x = 0; x < CMD_CHUNK_COUNT_X; x++)
    for(uint32_t y = 0; y < CMD_CHUNK_COUNT_Y; y++)
    for(uint32_t z = 0; z < CMD_CHUNK_COUNT_Z; z++)
    {
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
                chunk->blocks[CMD_GetParrayOffset(currentPos)]->blockID
                );
                dst->verticesTop++;
                }
                dst->iboTop+=6;
            }
        }
    }
    dst->ibo = CMD_ChunkIBO;
    CMD_PushChunkVBOData(dst);    
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