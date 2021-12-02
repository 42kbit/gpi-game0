#include "GPI_VertexArray.h"
#include "glad/glad.h"

GPI_VertexArray GPI_CreateVertexArray(GPI_VertexLayout* layout, GPI_Buffer* VBO, GPI_Buffer* IBO)
{
    uint32_t vaoID;
    glGenVertexArrays(1, &vaoID);
    return {vaoID, layout, VBO, IBO};
}

void GPI_BindVertexArrayAttribs(GPI_VertexArray* target)
{
    glBindVertexArray(target->ID);
    GPI_BindBuffer(target->VBO);
    GPI_BindBuffer(target->IBO);
    uint32_t offset = 0;
    for(uint32_t i = 0; i < target->dataLayout->segments.size(); i++)
    {
        auto& current = target->dataLayout->segments[i];
        switch (current.type)
        {
        case GL_FLOAT:
            glVertexAttribPointer(i, current.count, current.type, current.normalized, target->dataLayout->stride, (const void*)offset);
            break;
        case GL_UNSIGNED_INT:
            glVertexAttribIPointer(i, current.count, current.type, target->dataLayout->stride, (const void*)offset);
            break;
        }
        
        glEnableVertexAttribArray(i);    
        offset += current.count * GPI_GetGlTypeSize(current.type);
    }
}

void GPI_BindVertexArray(GPI_VertexArray* target)
{
    glBindVertexArray(target->ID);
}

void GPI_UnbindVertexArray(GPI_VertexArray* target)
{
    glBindVertexArray(0);
}