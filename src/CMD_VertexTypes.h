#ifndef __HEADER_CMD_VERTEXTYPES
#define __HEADER_CMD_VERTEXTYPES

#include "struct/vec2.h"
#include "struct/vec3.h"

#include "GPI_VertexLayout.h"

typedef struct 
{
    vec3 position;
    vec2 textureCoords;
    float texIndex;
} CMD_VertexDefault;
GPI_VertexLayout CMD_GetDefaultVertexLayout();
#endif /* __HEADER_CMD_VERTEXTYPES */
