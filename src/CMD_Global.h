#ifndef __HEADER_CMD_GLOBAL
#define __HEADER_CMD_GLOBAL

#include <inttypes.h>
#include "CMD_BlockType.h"
#include "GPI_Buffer.h"
#include "GPI_Shader.h"
#include "GPI_VertexLayout.h"
#include "GPI_Texture.h"

extern const uint8_t CMD_CHUNK_RENDER_DISTANCE;

#define CMD_CHUNK_RENDER_AREA (CMD_CHUNK_RENDER_DISTANCE * 2 + 1) * (CMD_CHUNK_RENDER_DISTANCE * 2 + 1) 

extern const uint32_t CMD_PROJECTION_VIEW_UNIFORMBLOCK_INDEX;

extern const uint8_t CMD_VOXEL_VERTECIES[];

extern GPI_Buffer CMD_ChunkIBO;
extern GPI_Shader CMD_ChunkShader;
extern GPI_VertexLayout CMD_ChunkLayout;

extern const CMD_BlockType CMD_AirBlock;
extern const CMD_BlockType CMD_GrassBlock;

#define CMD_MAX_TEXTURE_ATLASES 32
extern GPI_Texture* CMD_TextureAtlases;

extern void CMD_Init();
extern void CMD_Quit();

#endif /* __HEADER_CMD_GLOBAL */
