#define SDL_MAIN_HANDLED
#include "SDL.h"
#include "glad/glad.h"

#include "GPI_Buffer.h"
#include "GPI_Shader.h"
#include "GPI_Texture.h"
#include "GPI_Renderer.h"
#include "GPI_VertexArray.h"
#include "GPI_Camera.h"

#include <stddef.h>
#include <stdio.h>

#include "CMD_Input.h"
#include "CMD_Window.h"

#include "CMD_VertexTypes.h"
#include "CMD_BatchRenderer.h"

#include "CMD_Global.h"

// TODO: Add camera wrapper

static void GPI_SetUniformBlock(GPI_Shader* target, char* blockName, uint8_t index)
{
    uint32_t loc = glGetUniformBlockIndex(target->glID, blockName);
    if(loc != GL_INVALID_INDEX)
        glUniformBlockBinding(target->glID, loc, index);
    else
        exit(0);
}

const float voxelVertex[] = 
{
    //-z
    0,0,0,0,0,
    0,1,0,0,1,
    1,1,0,1,1,
    1,0,0,1,0,
    //+x
    1,0,0,0,0,
    1,1,0,0,1,
    1,1,1,1,1,
    1,0,1,1,0,
    //+z
    1,0,1,0,0,
    1,1,1,0,1,
    0,1,1,1,1,
    0,0,1,1,0,
    //-x
    0,0,1,0,0,
    0,1,1,0,1,
    0,1,0,1,1,
    0,0,0,1,0,
    //-y
    1,0,0,1,0,
    1,0,1,1,1,
    0,0,1,0,1,
    0,0,0,0,0,
    //+y
    1,1,1,1,1,
    1,1,0,1,0,
    0,1,0,0,0,
    0,1,1,0,1
};

int main(){
    SDL_Init(SDL_INIT_EVERYTHING);

    const uint32_t WIDTH = 800, HEIGHT = 600;
    const float aspectRaito = (float)WIDTH / HEIGHT;
    const uint32_t FPS_LIMIT = 120;

    SDL_Window* window = SDL_CreateWindow(
        "SDL window",
        SDL_WINDOWPOS_CENTERED, 
        SDL_WINDOWPOS_CENTERED, 
        WIDTH, 
        HEIGHT, 
        SDL_WINDOW_OPENGL);
    CMD_Input input = CMD_CreateInput();
    CMD_Window windowWrp = CMD_CreateWindow(window);
    int8_t shouldClose = 0;

    SDL_SetRelativeMouseMode(SDL_TRUE);

    SDL_GLContext context = SDL_GL_CreateContext(window);
    gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);
    gladLoadGL();

    glEnable(GL_DEPTH_TEST);

    GPI_Buffer ubo = GPI_CreateBuffer(GL_UNIFORM_BUFFER, sizeof(mat4)*2, NULL, GL_DYNAMIC_DRAW);

    GPI_Shader shaderProgram = GPI_CreateShaderFromFiles("res/shaders/defaultVertex.glsl", "res/shaders/textureDefault.glsl");
    vec3 campos = {0.f, 0.f, 3.f};
    GPI_Camera camera = GPI_CreateCamera(glm_rad(45.f), aspectRaito, campos);

    GPI_Texture tuxTexture = GPI_CreateTexture("res/textures/tux.png", GL_CLAMP_TO_BORDER, GL_NEAREST, GL_TEXTURE_2D);
    GPI_Texture catTexture = GPI_CreateTexture("res/textures/communist-cat.jpg", GL_CLAMP_TO_BORDER, GL_NEAREST, GL_TEXTURE_2D);    

    vec3 eulerCamRotation = {0.f, 0.f, 0.f};

    const float sensitivity = 0.1f;
    const float moveSpeed = 1.f;

    CMD_BatchData data = CMD_CreateBatchData(&shaderProgram);

    GPI_Buffer vbo = GPI_CreateBuffer(GL_ARRAY_BUFFER, 24 * sizeof(CMD_VertexDefault), NULL, GL_STATIC_DRAW);
    GPI_Buffer ibo = GPI_CreateBuffer(GL_ELEMENT_ARRAY_BUFFER, 36 * sizeof(uint32_t), NULL, GL_STATIC_DRAW);
    uint32_t* voxelInd = (uint32_t*)malloc(36 * sizeof(*voxelInd));
    for(uint32_t i = 0; i < 6; i++)
    {
        voxelInd[i*6+0] = i*4+0;
        voxelInd[i*6+1] = i*4+1;
        voxelInd[i*6+2] = i*4+2;
        voxelInd[i*6+3] = i*4+2;
        voxelInd[i*6+4] = i*4+3;
        voxelInd[i*6+5] = i*4+0;
    }
    GPI_BindBuffer(&ibo);
        glBufferSubData(ibo.TYPE, 0, 36 * sizeof(uint32_t), voxelInd);
    GPI_UnbindBuffer(&ibo);
    free(voxelInd);

    CMD_VertexDefault* vertecies = (CMD_VertexDefault*)malloc(24 * sizeof(CMD_VertexDefault));
    for(uint32_t i = 0; i < 24; i++)
    {
        memcpy(vertecies[i].position, voxelVertex+i*5 + 0, sizeof(vec3));
        memcpy(vertecies[i].textureCoords, voxelVertex+i*5 + 3, sizeof(vec2));
        vertecies[i].texIndex = 0.f;
    }
    GPI_BindBuffer(&vbo);
        glBufferSubData(vbo.TYPE, 0, 24 * sizeof(CMD_VertexDefault), vertecies);
    GPI_UnbindBuffer(&vbo);
    free(vertecies);

    GPI_VertexLayout layout = CMD_GetDefaultVertexLayout();
    GPI_VertexArray vao = GPI_CreateVertexArray(&layout, &vbo, &ibo);
    GPI_BindVertexArray(&vao);
    GPI_BindVertexArrayAttribs(&vao);

    while(!shouldClose)
    {
        uint32_t lastTime = SDL_GetTicks();

        GPI_SetCameraRotation(&camera, aspectRaito, 
        glm_rad(eulerCamRotation[0]), 
        glm_rad(eulerCamRotation[1]), 
        glm_rad(eulerCamRotation[2]));
        
        vec3 camdx; glm_vec3_scale(camera.forward, moveSpeed * 0.f, camdx);
        GPI_MoveCamera(&camera, camdx);

        memset(input.deltaMouse, 0, sizeof(vec2));

        mat4 proj; GPI_GetCameraProjection(&camera, aspectRaito, proj);
        mat4 view; GPI_GetCameraView(&camera, view);
        
        GPI_BindBuffer(&ubo);
            glBufferSubData(ubo.TYPE, 0, sizeof(mat4), &proj);
            glBufferSubData(ubo.TYPE, sizeof(mat4), sizeof(mat4), &view);
        GPI_UnbindBuffer(&ubo);
    
        CMD_PollEvents(&input);
        if(input.pressed[SDL_SCANCODE_ESCAPE])
            shouldClose = 1;
        if(input.pressed[SDL_SCANCODE_W]) {
            vec3 mv = {camera.forward[0], 0, camera.forward[2]};
            glm_vec3_scale(mv, moveSpeed * windowWrp.deltaTime, mv);
            GPI_MoveCamera(&camera, mv);
        }
        if(input.pressed[SDL_SCANCODE_S]) {
            vec3 mv = {camera.forward[0], 0, camera.forward[2]};
            glm_vec3_scale(mv, moveSpeed * windowWrp.deltaTime, mv);
            glm_vec3_negate(mv);
            GPI_MoveCamera(&camera, mv);
        }
        if(input.pressed[SDL_SCANCODE_D]) {
            vec3 mv = {camera.right[0], 0, camera.right[2]};
            glm_vec3_scale(mv, moveSpeed * windowWrp.deltaTime, mv);
            GPI_MoveCamera(&camera, mv);
        }
        if(input.pressed[SDL_SCANCODE_A]) {
            vec3 mv = {camera.right[0], 0, camera.right[2]};
            glm_vec3_scale(mv, moveSpeed * windowWrp.deltaTime, mv);
            glm_vec3_negate(mv);
            GPI_MoveCamera(&camera, mv);
        }
        if(input.pressed[SDL_SCANCODE_SPACE]) {
            vec3 mv = {0, moveSpeed * windowWrp.deltaTime, 0};
            GPI_MoveCamera(&camera, mv);
        }
        if(input.pressed[SDL_SCANCODE_LSHIFT]) {
            vec3 mv = {0, moveSpeed * windowWrp.deltaTime, 0};
            glm_vec3_negate(mv);
            GPI_MoveCamera(&camera, mv);
        }
        if(input.pressed[SDL_SCANCODE_TAB])
            glEnable(GL_CULL_FACE);
        else
            glDisable(GL_CULL_FACE);

        eulerCamRotation[0] += -input.deltaMouse[1] * sensitivity;
        eulerCamRotation[1] += -input.deltaMouse[0] * sensitivity;

        //rendering
        glClearColor(0.7,1,1,1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        int32_t samplers[32];
        for(uint32_t i = 0; i < 32; ++i)
            samplers[i] = i;
        int32_t loc = GPI_GetUniformLocation(&shaderProgram, "u_Textures");
        if(loc != -1)
            glUniform1iv(loc, 32, samplers);
        glBindTextureUnit(0, catTexture.glID);
        glBindTextureUnit(1, tuxTexture.glID);

        const uint8_t index = 0;
        glBindBufferRange(ubo.TYPE, index, ubo.glID, 0, sizeof(mat4)*2);

        GPI_SetUniformBlock(&shaderProgram, "ProjectionView", index);
        
        mat4 model; glm_mat4_identity(model);
        loc = GPI_GetUniformLocation(&shaderProgram, "u_Model");
        if(loc != -1)
            glUniformMatrix4fv(loc, 1, GL_FALSE, model);

        GPI_BindVertexArray(&vao);
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, NULL);
        GPI_UnbindVertexArray(&vao);

        //CMD_BeginBatch(&data);
        //vec3 p0 = {0,0,0};
        //vec3 p1 = {1,0,0};
        //CMD_PushQuadData(&data, p0, &catTexture);
        //CMD_PushQuadData(&data, p1, &tuxTexture);
        //CMD_EndBatch(&data);
        //CMD_Flush(&data);

        SDL_GL_SwapWindow(window);

        uint32_t currentTime = SDL_GetTicks();    
        uint32_t tickDiff = currentTime - lastTime;
        if((tickDiff) < (1000 / FPS_LIMIT))
            SDL_Delay((1000 / FPS_LIMIT) - (tickDiff));
        windowWrp.deltaTime = (float)(SDL_GetTicks() - lastTime) / 1000.f;
    }

    GPI_FreeLayout(&layout);
    CMD_FreeBatchData(&data);
    SDL_DestroyWindow(window);
    SDL_Quit();
}