#ifndef _HEADER__CMD_BATCHRENDERER
#define _HEADER__CMD_BATCHRENDERER

#include <inttypes.h>
#include "CMD_VertexTypes.h"

#include "GPI_Buffer.h"
#include "GPI_VertexArray.h"

#include "GPI_Shader.h"
/*
    _PB = per batch
*/
const uint32_t CMD_MAX_QUADS_PB = 64;
const uint32_t CMD_MAX_INDECIES_PB = CMD_MAX_QUADS_PB * 6;
const uint32_t CMD_MAX_VERTECIES_PB = CMD_MAX_QUADS_PB * 4;
const uint8_t CMD_QUADSIZE = 1;

struct CMD_BatchData
{
    GPI_VertexArray vao;

    GPI_Buffer vbo;
    GPI_Buffer ibo;

    GPI_VertexLayout layout;

    GPI_Shader* externalShader;

    CMD_VertexDefault* vertexArray = NULL;
    CMD_VertexDefault* vertexArrayItr = NULL;
};

CMD_BatchData CMD_CreateBatchData(GPI_Shader* externalShader);

void CMD_PushQuadData(CMD_BatchData* target, glm::vec3 position); // add an rotation later

void CMD_BeginBatch(CMD_BatchData* target);
void CMD_Flush(CMD_BatchData* target);
void CMD_EndBatch(CMD_BatchData* target);

#endif /* _HEADER__CMD_BATCHRENDERER */
