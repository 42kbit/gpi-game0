#include "GPI_Buffer.h"
#include "glad/glad.h"

GPI_Buffer GPI_CreateBuffer(uint32_t TYPE, uint32_t byte_size, void* data, uint32_t DRAW_MODE)
{
    uint32_t id;
    glGenBuffers(1, &id);
    glBindBuffer(TYPE, id);
    glBufferData(TYPE, byte_size, data, DRAW_MODE);
    glBindBuffer(TYPE, 0);
    GPI_Buffer buffer = {id, TYPE, DRAW_MODE};
    return buffer;
}

void GPI_BindBuffer(GPI_Buffer* buffer)
{
    glBindBuffer(buffer->TYPE, buffer->glID);
}

void GPI_UnbindBuffer(GPI_Buffer* buffer)
{
    glBindBuffer(buffer->TYPE, 0);
}
