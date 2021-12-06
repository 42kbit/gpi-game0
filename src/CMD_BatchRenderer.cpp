#include "CMD_BatchRenderer.h"

#include "glad/glad.h"

#include <cstring>

CMD_BatchData CMD_CreateBatchData(GPI_Shader* shader)
{
    CMD_BatchData data;

    data.vertexArray = new CMD_VertexDefault[CMD_MAX_VERTECIES_PB];
    memset(data.vertexArray, 0, CMD_MAX_VERTECIES_PB * sizeof(CMD_VertexDefault));
    data.vertexArrayItr = data.vertexArray;
    uint32_t* indecies = new uint32_t[CMD_MAX_INDECIES_PB];

    for(uint32_t i = 0; i < CMD_MAX_INDECIES_PB/6; ++i)
    {
        indecies[i*6 + 0] = i*4 + 0;
        indecies[i*6 + 1] = i*4 + 1;
        indecies[i*6 + 2] = i*4 + 2;
        indecies[i*6 + 3] = i*4 + 2;
        indecies[i*6 + 4] = i*4 + 3;
        indecies[i*6 + 5] = i*4 + 0;
    }

    data.vbo = GPI_CreateBuffer(GL_ARRAY_BUFFER, sizeof(CMD_VertexDefault) * CMD_MAX_VERTECIES_PB, NULL, GL_DYNAMIC_DRAW);
    data.ibo = GPI_CreateBuffer(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * CMD_MAX_INDECIES_PB, indecies, GL_STATIC_DRAW);
    data.layout = CMD_GetDefaultVertexLayout();
    data.vao = GPI_CreateVertexArray(&data.layout, &data.vbo, &data.ibo);
    GPI_BindVertexArray(&data.vao);
    GPI_BindVertexArrayAttribs(&data.vao);

    GPI_BindVertexArray(&data.vao);
    GPI_BindBuffer(&data.vbo);
    GPI_BindBuffer(&data.ibo);
    GPI_UnbindVertexArray(&data.vao);

    data.externalShader = shader;

    return data;
}

void CMD_PushQuadData(CMD_BatchData* target, glm::vec3 position)
{
    if((target->vertexArrayItr - target->vertexArray) >= CMD_MAX_VERTECIES_PB)
    {
        CMD_EndBatch(target);
        CMD_Flush(target);
        CMD_BeginBatch(target);
    }

    *(target->vertexArrayItr++) = {position, {0.f, 0.f}, 0.f};
    *(target->vertexArrayItr++) = {glm::vec3(position.x + CMD_QUADSIZE, position.y, position.z), {1.f, 0.f}, 0.f};
    *(target->vertexArrayItr++) = {glm::vec3(position.x + CMD_QUADSIZE, position.y + CMD_QUADSIZE, position.z), {1.f, 1.f}, 0.f};
    *(target->vertexArrayItr++) = {glm::vec3(position.x, position.y + CMD_QUADSIZE, position.z), {0.f, 1.f}, 0.f};
}

void CMD_BeginBatch(CMD_BatchData* target){
    target->vertexArrayItr = target->vertexArray;
}

void CMD_Flush(CMD_BatchData* target)
{
    GPI_BindVertexArray(&target->vao);
    GPI_BindShader(target->externalShader);
    glDrawElements(GL_TRIANGLES, CMD_MAX_INDECIES_PB, GL_UNSIGNED_INT, NULL);
}

void CMD_EndBatch(CMD_BatchData* target)
{
    uint32_t arraysOffset = (target->vertexArrayItr - target->vertexArray) * sizeof(CMD_VertexDefault);
    GPI_BindBuffer(&target->vbo);
    glBufferSubData(target->vbo.TYPE, 0, arraysOffset, target->vertexArray);
}