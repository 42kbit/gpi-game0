#include "CMD_VertexTypes.h"

GPI_VertexLayout CMD_GetDefaultVertexLayout()
{
    GPI_VertexLayout result = GPI_CreateVertexLayout();
    GPI_PushLayoutData(&result, 3, GL_FLOAT, GL_FALSE);
    GPI_PushLayoutData(&result, 2, GL_FLOAT, GL_FALSE);
    GPI_PushLayoutData(&result, 1, GL_FLOAT, GL_FALSE);
    return result;
}