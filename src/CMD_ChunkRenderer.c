#include "CMD_ChunkRenderer.h"
#include "CMD_Global.h"
#include "GPI_VertexArray.h"

#include <string.h>

#include "project.h"

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

static void CMD_ScreenToOpenGL(vec2 screenPos, GPI_Camera* camera, mat4 model, vec3 dst)
{
    int32_t glvp[4];
    glGetIntegerv(GL_VIEWPORT, glvp);
    float aspectRaito = (float)glvp[2] / (float)glvp[3]; 
    vec4 viewPort = {glvp[0], glvp[1], glvp[2], glvp[3]};
    mat4 proj; GPI_GetCameraProjection(camera, aspectRaito, proj);
    mat4 view; GPI_GetCameraView(camera, view);
    mat4 pv; glm_mat4_mul(proj, view, pv);
    mat4 m; glm_mat4_identity(m);  
    if(model != NULL) 
        memcpy(m, model, sizeof(mat4));
    mat4 pvm; glm_mat4_mul(pv, m, pvm);
    float depth;
    float ycoord = viewPort[3] - screenPos[1] - 1;
    glReadPixels(screenPos[0],  ycoord, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
    vec3 mposf = {screenPos[0], ycoord, depth};
    
    glm_unproject(mposf, pvm, viewPort, dst);
}

#define MIN(a,b) (a > b? b : a)
#define MAX(a,b) (a > b? a : b)

static float CMD_GetMin4F(vec4 value){
    float result = 10000.f;
    for(uint32_t i = 0; i < 3; i++)
    {
        result = MIN(value[i], value[i+1]);
    }
}

void CMD_SetBlockWorldSpace(
    CMD_ChunkMesh* mesh, 
    CMD_Chunk* chunk, 
    vec2 viewportPos, 
    GPI_Camera* camera, 
    CMD_BlockType* block, 
    CMD_BLOCKSETFUNC placementFunc)
{
    vec3 blockPos;
    CMD_ScreenToOpenGL(viewportPos, camera, NULL, blockPos);
    vec3 dst;
    vec3 scaledCam;
    glm_vec3_scale(camera->forward, 0.1f, scaledCam);
    if(block->flags & CMD_BLOCK_NEGATION_MASK)
        glm_vec3_negate(scaledCam);
    glm_vec3_add(blockPos, scaledCam, dst);
    placementFunc(mesh, chunk, dst, block);
}

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
        if(CMD_IsInChunk(dst))
            CMD_RecalculateMesh(mesh, chunk, dst);
    }
    CMD_PushChunkVBOData(mesh);
}

static void GPI_SetUniformBlock(GPI_Shader* target, char* blockName, uint8_t index)
{
    uint32_t loc = glGetUniformBlockIndex(target->glID, blockName);
    if(loc != GL_INVALID_INDEX)
        glUniformBlockBinding(target->glID, loc, index);
}

void CMD_RenderChunkMesh(CMD_ChunkMesh* mesh, vec3 pos)
{
    GPI_BindVertexArray(&mesh->vao);
    GPI_BindVertexArrayAttribs(&mesh->vao);
    GPI_BindBuffer(&mesh->vbo);
    GPI_BindBuffer(&mesh->ibo);

    GPI_BindShader(&CMD_ChunkShader);
    int32_t samplers[32];
    for(uint32_t i = 0; i < 32; ++i)
        samplers[i] = i;
    int32_t loc = GPI_GetUniformLocation(&CMD_ChunkShader, "u_Textures");
    if(loc != -1)
        glUniform1iv(loc, 32, samplers);
    glBindTextureUnit(0, CMD_TextureAtlases[0].glID);
    const uint8_t index = 0;
    glBindBufferRange(
        CMD_ProjectionViewUniformBlock.TYPE, 
        index, 
        CMD_ProjectionViewUniformBlock.glID, 
        0, 
        sizeof(mat4)*2);
    GPI_SetUniformBlock(&CMD_ChunkShader, "ProjectionView", index);
    
    mat4 model; glm_mat4_identity(model);
    loc = GPI_GetUniformLocation(&CMD_ChunkShader, "u_Model");
    if(loc != -1)
        glUniformMatrix4fv(loc, 1, GL_FALSE, model);
    loc = GPI_GetUniformLocation(&CMD_ChunkShader, "u_ChunkPosition");
    if(loc != -1)
        glUniform3f(loc, pos[0], pos[1], pos[2]);
    glDrawElements(
        GL_TRIANGLES,
        mesh->iboTop, 
        GL_UNSIGNED_INT, 
        NULL);
}