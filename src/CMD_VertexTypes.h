#ifndef _HEADER__CMD_VERTEXTYPES
#define _HEADER__CMD_VERTEXTYPES

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"

#include "GPI_VertexLayout.h"

struct CMD_VertexDefault
{
    glm::vec3 position;
    glm::vec2 textureCoords;
    float texIndex;
};
GPI_VertexLayout CMD_GetDefaultVertexLayout();
#endif /* _HEADER__CMD_VERTEXTYPES */
