#include "GPI_Renderer.h"

#include "glad/glad.h"

void GPI_DrawBuffers(GPI_Buffer* vertexBuffer, GPI_Buffer* indexBuffer, uint32_t indexBufferCnt, GPI_VertexLayout* layout)
{
    GPI_BindBuffer(vertexBuffer);
    for(uint32_t i = 0; i < layout->segments.size(); i++)
    {
        auto& currentSegment = layout->segments[i];
        glVertexAttribPointer(i, currentSegment.count, currentSegment.type, currentSegment.normalized, layout->stride, NULL);
    }
    GPI_UnbindBuffer(vertexBuffer);

    GPI_BindBuffer(indexBuffer);
    glDrawElements(GL_TRIANGLES, indexBufferCnt, GL_UNSIGNED_INT, NULL);
    GPI_UnbindBuffer(indexBuffer);
}

