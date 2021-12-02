#ifndef _HEADER__GPI_RENDERER
#define _HEADER__GPI_RENDERER
#include "GPI_Buffer.h"
#include "GPI_Shader.h"
#include "GPI_VertexLayout.h"

void GPI_DrawBuffers(GPI_Buffer* vertexBuffer, GPI_Buffer* indexBuffer, uint32_t indexBufferCnt, GPI_VertexLayout* layout);


#endif /* _HEADER__GPI_RENDERER */
