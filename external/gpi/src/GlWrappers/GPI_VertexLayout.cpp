#include "GPI_VertexLayout.h"

void GPI_PushLayoutData(GPI_VertexLayout* layout, uint32_t count, uint32_t TYPE, uint32_t NORMALIZED) 
{
    GPI_VertexSegment segment;
    segment.count = count;
    segment.type = TYPE;
    segment.normalized = NORMALIZED;
    layout->segments.push_back(segment);
    layout->stride += GPI_GetGlTypeSize(TYPE) * count;
}
