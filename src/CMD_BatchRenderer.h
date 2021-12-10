#ifndef __HEADER_CMD_BATCHRENDERER
#define __HEADER_CMD_BATCHRENDERER

#include <inttypes.h>
#include "CMD_VertexTypes.h"

#include "GPI_Buffer.h"
#include "GPI_VertexArray.h"

#include "GPI_Shader.h"
#include "GPI_Texture.h"
/*
    _PB = per batch
*/
#define CMD_MAX_QUADS_PB 64
#define CMD_MAX_INDECIES_PB CMD_MAX_QUADS_PB * 6
#define CMD_MAX_VERTECIES_PB CMD_MAX_QUADS_PB * 4
#define CMD_MAX_TEXTURES_PB 32
#define CMD_QUADSIZE 1

typedef struct
{
    GPI_VertexArray vao;

    GPI_Buffer vbo;
    GPI_Buffer ibo;

    GPI_VertexLayout layout;

    GPI_Shader* externalShader;
    GPI_Texture** textures;
    uint32_t texturesTop;

    CMD_VertexDefault* vertexArray;
    CMD_VertexDefault* vertexArrayItr;
} CMD_BatchData;

CMD_BatchData CMD_CreateBatchData(GPI_Shader* externalShader);
void CMD_FreeBatchData(CMD_BatchData* target);

void CMD_PushQuadData(CMD_BatchData* target, vec3 position, GPI_Texture* texture); // add an rotation later

void CMD_BeginBatch(CMD_BatchData* target);
void CMD_Flush(CMD_BatchData* target);
void CMD_EndBatch(CMD_BatchData* target);

#endif /* __HEADER_CMD_BATCHRENDERER */
