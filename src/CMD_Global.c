#include "CMD_Global.h"
#include "CMD_Chunk.h"
#include "CMD_VertexTypes.h"
#include <malloc.h>

GPI_Buffer CMD_ChunkIBO;
GPI_Shader CMD_ChunkShader;
GPI_VertexLayout CMD_ChunkLayout;

GPI_Texture* CMD_TextureAtlases;

void CMD_Init()
{
    uint32_t* voxelInd = (uint32_t*)malloc(36*CMD_CHUNK_COUNT_ALL * sizeof(*voxelInd));
    for(uint32_t i = 0; i < 6*CMD_CHUNK_COUNT_ALL; i++)
    {
        voxelInd[i*6+0] = i*4+0;
        voxelInd[i*6+1] = i*4+1;
        voxelInd[i*6+2] = i*4+2;
        voxelInd[i*6+3] = i*4+2;
        voxelInd[i*6+4] = i*4+3;
        voxelInd[i*6+5] = i*4+0;
    }
    CMD_ChunkIBO = GPI_CreateBuffer(
        GL_ELEMENT_ARRAY_BUFFER, 
        36 * CMD_CHUNK_COUNT_ALL * sizeof(uint32_t),
        voxelInd, 
        GL_STATIC_DRAW
        );
    free(voxelInd);
    CMD_ChunkShader = GPI_CreateShaderFromFiles(
        "res/shaders/chunkVertex.glsl", 
        "res/shaders/chunkTexture.glsl");
    CMD_ChunkLayout = CMD_GetChunkVertexLayout();
    CMD_TextureAtlases = (GPI_Texture*)malloc(CMD_MAX_TEXTURE_ATLASES * sizeof(*CMD_TextureAtlases));
    CMD_TextureAtlases[0] = GPI_CreateTexture("res/textures/atlas0.png", GL_CLAMP_TO_BORDER, GL_NEAREST, GL_TEXTURE_2D);
}

void CMD_Quit()
{
    GPI_FreeLayout(&CMD_ChunkLayout);
    free(CMD_TextureAtlases);
}

const uint8_t CMD_CHUNK_RENDER_DISTANCE = 0;

const uint32_t CMD_PROJECTION_VIEW_UNIFORMBLOCK_INDEX = 0;
const uint32_t CMD_CHUNK_RENDERER_SLICE_COUNT = 4;

const CMD_BlockType CMD_AirBlock = {CMD_BLOCKID_AIR, 1};
const CMD_BlockType CMD_GrassBlock = {CMD_BLOCKID_GRASS, 0};
const uint8_t CMD_VOXEL_VERTECIES[] = 
{ 
    //+x
    1,0,0,0, //TODO: add texture id 
    1,1,0,1,
    1,1,1,2,
    1,0,1,3,    
    //-x
    0,0,1,0,
    0,1,1,1,
    0,1,0,2,
    0,0,0,3,
    //+y
    1,1,1,2,
    1,1,0,3,
    0,1,0,0,
    0,1,1,1,
    //-y
    1,0,0,3,
    1,0,1,2,
    0,0,1,1,
    0,0,0,0,
    //+z
    1,0,1,0,
    1,1,1,1,
    0,1,1,2,
    0,0,1,3,
    //-z
    0,0,0,0,
    0,1,0,1,
    1,1,0,2,
    1,0,0,3
};