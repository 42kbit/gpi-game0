#ifndef __HEADER_GPI_VERTEXARRAY
#define __HEADER_GPI_VERTEXARRAY

#include "GPI_Buffer.h"
#include "GPI_VertexLayout.h"

typedef struct
{
    uint32_t ID;
    GPI_VertexLayout* dataLayout;
    GPI_Buffer* VBO;
    GPI_Buffer* IBO;
} GPI_VertexArray;

GPI_VertexArray GPI_CreateVertexArray(GPI_VertexLayout* layout, GPI_Buffer* VBO, GPI_Buffer* IBO);
void GPI_BindVertexArrayAttribs(GPI_VertexArray* target);
void GPI_BindVertexArray(GPI_VertexArray* target);
void GPI_UnbindVertexArray(GPI_VertexArray* target);

#endif /* __HEADER_GPI_VERTEXARRAY */
