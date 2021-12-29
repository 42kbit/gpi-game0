#include "CMD_ChunkRenderer.h"
#include "CMD_Global.h"
#include "GPI_VertexArray.h"


#include <string.h>

static vec3 opx = {1,0,0};
static vec3 onx = {-1,0,0};
static vec3 opy = {0,1,0};
static vec3 ony = {0,-1,0};
static vec3 opz = {0,0,1};
static vec3 onz = {0,0,-1}; 
static vec3* direcions[6] = {opx, onx, opy, ony, opz, onz};

static uint8_t CMD_DrawSide(CMD_Chunk* c, vec3 pos, vec3 offset)
{
    if(!CMD_IsInChunkOffset(pos, offset)) return c->blocks[CMD_GetParrayOffset(pos)]->flags?  0 : 1;
    uint32_t currentBlockpos = (uint32_t)(pos[0])*CMD_CHUNK_COUNT_Y*CMD_CHUNK_COUNT_Z + (uint32_t)(pos[1])*CMD_CHUNK_COUNT_Z +(uint32_t)(pos[2]);
    uint32_t arraypos = (uint32_t)(pos[0]+offset[0])*CMD_CHUNK_COUNT_Y*CMD_CHUNK_COUNT_Z + (uint32_t)(pos[1]+offset[1])*CMD_CHUNK_COUNT_Z +(uint32_t)(pos[2]+offset[2]);
    return !(c->blocks[currentBlockpos]->flags & CMD_BLOCK_TRANSPARENCY_MASK) && (c->blocks[arraypos]->flags & CMD_BLOCK_TRANSPARENCY_MASK);
}

static CMD_ChunkVertex* CMD_FindQuadInVertexBuffer(CMD_ChunkVertex* buffer, uint32_t count, CMD_ChunkVertex quad[4])
{
    for(uint32_t i = 0; i < count/4; i++)
    {
        uint32_t verteciesCounted = 0;
        for(uint32_t j = 0; j < 4; j++)
        {
            uint32_t px = CMD_UNMAP_CHUNK_POSX((buffer+i*4+j)->data0);
            uint32_t py = CMD_UNMAP_CHUNK_POSX((buffer+i*4+j)->data0);
            uint32_t pz = CMD_UNMAP_CHUNK_POSX((buffer+i*4+j)->data0);
            if(CMD_UNMAP_CHUNK_POSX((buffer+i*4+j)->data0) == CMD_UNMAP_CHUNK_POSX(quad[j].data0) && 
               CMD_UNMAP_CHUNK_POSY((buffer+i*4+j)->data0) == CMD_UNMAP_CHUNK_POSY(quad[j].data0) &&
               CMD_UNMAP_CHUNK_POSZ((buffer+i*4+j)->data0) == CMD_UNMAP_CHUNK_POSZ(quad[j].data0))
                verteciesCounted++; 
        }
        if(verteciesCounted >= 4)
            return buffer+i*4;
    }
    return NULL;
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

static void CMD_RemoveQuadFromMesh(CMD_ChunkMesh* target, CMD_ChunkVertex* pLocation)
{
    memset(pLocation, 0, sizeof(CMD_ChunkVertex)*4);
    target->iboTop -= 6;
    target->verticesTop -= 4;
}

static void CMD_RecalculateMesh(CMD_ChunkMesh* mesh, CMD_Chunk* chunk, vec3 pos)
{
    CMD_ChunkVertex vertecies[24];
    memset(vertecies, 0, sizeof(CMD_ChunkVertex)*4);
    for(uint32_t i = 0; i < 24; i++)
    {
        vertecies[i] = CMD_MapChunkVertexData(
            CMD_VOXEL_VERTECIES[i*4+0]+(uint32_t)pos[0],
            CMD_VOXEL_VERTECIES[i*4+1]+(uint32_t)pos[1],
            CMD_VOXEL_VERTECIES[i*4+2]+(uint32_t)pos[2],
            CMD_VOXEL_VERTECIES[i*4+3],
            0
        );
    }
    
    for(uint32_t i = 0; i < 6; i++)
    {
        CMD_ChunkVertex quad[4];
        memcpy(quad, vertecies + i*4, sizeof(CMD_ChunkVertex)*4);
        CMD_ChunkVertex* pBufferLoc = CMD_FindQuadInVertexBuffer(mesh->vertices, mesh->verticesTop, quad);
        if(pBufferLoc != NULL)
        {
            uint32_t oldTop = mesh->verticesTop;
            CMD_RemoveQuadFromMesh(mesh, pBufferLoc);
            CMD_ReassignVertexArray(mesh->vertices, oldTop);
        }
        if(CMD_DrawSide(chunk, pos, direcions[i]))
        {
            for(uint32_t j = 0; j < 4; j++){
            mesh->vertices[mesh->verticesTop] = CMD_MapChunkVertexData(
            CMD_VOXEL_VERTECIES[j*4+0+i*16] +pos[0],
            CMD_VOXEL_VERTECIES[j*4+1+i*16] +pos[1],
            CMD_VOXEL_VERTECIES[j*4+2+i*16] +pos[2],
            CMD_VOXEL_VERTECIES[j*4+3+i*16], 
            chunk->blocks[CMD_GetParrayOffset(pos)]->blockID
            );
            mesh->verticesTop++;
            }
            mesh->iboTop+=6;
        }
    }
};

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
                CMD_VOXEL_VERTECIES[j*4+0+i*16] +x,
                CMD_VOXEL_VERTECIES[j*4+1+i*16] +y,
                CMD_VOXEL_VERTECIES[j*4+2+i*16] +z,
                CMD_VOXEL_VERTECIES[j*4+3+i*16], 
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

void CMD_SetBlockRegenerate(CMD_ChunkMesh* mesh, CMD_Chunk* chunk, vec3 pos, CMD_BlockType* block)
{
    if(!CMD_IsInChunk(pos))
        return;
    uint32_t index = CMD_GetParrayOffset(pos);
    if(chunk->blocks[index] == block)
        return;
    chunk->blocks[index] = block;
    CMD_RegenerateChunkMesh(mesh, chunk);
}

void CMD_SetBlockUpdatable(CMD_ChunkMesh* mesh, CMD_Chunk* chunk, vec3 pos, CMD_BlockType* block)
{
    if(!CMD_IsInChunk(pos))
        return;
    uint32_t index = CMD_GetParrayOffset(pos);
    if(chunk->blocks[index] == block)
        return;
    chunk->blocks[index] = block;
    CMD_RecalculateMesh(mesh, chunk, pos);
    for(uint32_t i = 0; i < 6; i++)
    {
        vec3 dst;
        glm_vec3_add(pos, direcions[i], dst);
        CMD_RecalculateMesh(mesh, chunk, dst);
    }
    CMD_PushChunkVBOData(mesh);
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