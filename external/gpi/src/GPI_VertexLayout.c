#include "GPI_VertexLayout.h"

#include <malloc.h>

uint32_t GPI_GetGlTypeSize(uint32_t TYPE)
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

void GPI_PushLayoutData(GPI_VertexLayout* layout, uint32_t count, uint32_t TYPE, uint32_t NORMALIZED) 
{
    GPI_VertexSegment segment;
    segment.count = count;
    segment.type = TYPE;
    segment.normalized = NORMALIZED;
    layout->segments[layout->segmentsTop++] = segment;
    layout->stride += GPI_GetGlTypeSize(TYPE) * count;
}

GPI_VertexLayout GPI_CreateVertexLayout()
{
    GPI_VertexLayout layout = {0, malloc(sizeof(GPI_VertexSegment) * GPI_MAX_VERTEX_LAYOUT_SEGMENTS), 0};
    return layout;
}

void GPI_FreeLayout(GPI_VertexLayout* target)
{
    free(target->segments);
}
