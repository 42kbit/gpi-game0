#ifndef _HEADER__GPI_VERTEXLAYOUT
#define _HEADER__GPI_VERTEXLAYOUT

#include "glad/glad.h"

#include <vector>
#include "GPI_Buffer.h"

inline uint32_t GPI_GetGlTypeSize(uint32_t TYPE)
{
    switch (TYPE)
    {
    case GL_FLOAT:
    case GL_UNSIGNED_INT:
        return 4;
        break;
    }
    return 0;
}

struct GPI_VertexSegment
{
    uint32_t count;
    uint32_t type;
    uint32_t normalized;
};

struct GPI_VertexLayout
{
    uint32_t stride = 0;
    std::vector<GPI_VertexSegment> segments;
};

void GPI_PushLayoutData(GPI_VertexLayout* layout, uint32_t count, uint32_t TYPE, uint32_t NORMALIZED);


#endif /* _HEADER__GPI_VERTEXLAYOUT */
