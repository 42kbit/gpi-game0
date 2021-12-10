#include "CMD_BatchRenderer.h"

#include "glad/glad.h"

#include <string.h>
#include <malloc.h>

CMD_BatchData CMD_CreateBatchData(GPI_Shader* shader)
{
    CMD_BatchData data;

    data.vertexArray = (CMD_VertexDefault*)malloc(CMD_MAX_VERTECIES_PB * sizeof(*data.vertexArray));
    memset(data.vertexArray, 0, CMD_MAX_VERTECIES_PB * sizeof(CMD_VertexDefault));
    data.textures = (GPI_Texture**)malloc(CMD_MAX_TEXTURES_PB * sizeof(*data.textures));
    data.texturesTop = 0;
    data.vertexArrayItr = data.vertexArray;
    uint32_t* indecies = (uint32_t*)malloc(CMD_MAX_INDECIES_PB * sizeof(*indecies));
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
    
    free(indecies);

    GPI_BindVertexArray(&data.vao);
    GPI_BindVertexArrayAttribs(&data.vao);

    GPI_BindVertexArray(&data.vao);
    GPI_BindBuffer(&data.vbo);
    GPI_BindBuffer(&data.ibo);
    GPI_UnbindVertexArray(&data.vao);

    data.externalShader = shader;
    GPI_BindShader(data.externalShader);
    int32_t texInd[CMD_MAX_TEXTURES_PB];
    for(uint32_t i = 0; i < CMD_MAX_TEXTURES_PB; i++)
        texInd[i] = i;
    int32_t loc = GPI_GetUniformLocation(data.externalShader, "u_Textures");
    if(loc != -1)
        glUniform1iv(loc, CMD_MAX_TEXTURES_PB, texInd);

    memset(data.textures, 0, CMD_MAX_TEXTURES_PB*sizeof(data.textures[0]));

    return data;
}

void CMD_FreeBatchData(CMD_BatchData* target)
{
    free(target->vertexArray);
    free(target->textures);
    GPI_FreeLayout(&target->layout);
}

void CMD_PushQuadData(CMD_BatchData* target, vec3 position, GPI_Texture* texture)
{
    if((target->vertexArrayItr - target->vertexArray) >= CMD_MAX_VERTECIES_PB || target->texturesTop >= CMD_MAX_TEXTURES_PB-1)
    {
        CMD_EndBatch(target);
        CMD_Flush(target);
        CMD_BeginBatch(target);
    }

    uint32_t textureIndex = 0;
    for(uint32_t i = 0; i < target->texturesTop; i++)
    {
        if(target->textures[i] == texture)
        {
            textureIndex = i;
            break;
        }
    }
    if(textureIndex == 0)
    {
        textureIndex = target->texturesTop;
        target->textures[target->texturesTop++] = texture;
    }
    CMD_VertexDefault v0 = { {position[0], position[1], position[2]}, {0.f, 0.f}, (float)textureIndex };
    CMD_VertexDefault v1 = { {position[0] + CMD_QUADSIZE, position[1], position[2]}, {1.f, 0.f}, (float)textureIndex };
    CMD_VertexDefault v2 = { {position[0] + CMD_QUADSIZE, position[1] + CMD_QUADSIZE, position[2]}, {1.f, 1.f}, (float)textureIndex };
    CMD_VertexDefault v3 = { {position[0], position[1] + CMD_QUADSIZE, position[2]}, {0.f, 1.f}, (float)textureIndex };
    *(target->vertexArrayItr++) = v0;
    *(target->vertexArrayItr++) = v1;
    *(target->vertexArrayItr++) = v2;
    *(target->vertexArrayItr++) = v3;
}

void CMD_BeginBatch(CMD_BatchData* target){
    target->vertexArrayItr = target->vertexArray;
}

void CMD_Flush(CMD_BatchData* target)
{
    GPI_BindVertexArray(&target->vao);
    GPI_BindShader(target->externalShader);

    for(uint32_t i = 0; i < target->texturesTop; i++)
        glBindTextureUnit(i, target->textures[i]->glID);

    glDrawElements(GL_TRIANGLES, CMD_MAX_INDECIES_PB, GL_UNSIGNED_INT, NULL);
    target->texturesTop = 0;
}

void CMD_EndBatch(CMD_BatchData* target)
{
    uint32_t arraysOffset = (target->vertexArrayItr - target->vertexArray) * sizeof(CMD_VertexDefault);
    GPI_BindBuffer(&target->vbo);
    glBufferSubData(target->vbo.TYPE, 0, arraysOffset, target->vertexArray);
}