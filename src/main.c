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
#include <stdlib.h>

#include "CMD_Input.h"
#include "CMD_Window.h"

#include "CMD_VertexTypes.h"
#include "CMD_BatchRenderer.h"

#include "CMD_Chunk.h"

#include "CMD_Global.h"
#include "CMD_Meshes.h"

#include "CMD_ChunkRenderer.h"

#include "project.h"

static void GPI_SetUniformBlock(GPI_Shader* target, char* blockName, uint8_t index)
{
    uint32_t loc = glGetUniformBlockIndex(target->glID, blockName);
    if(loc != GL_INVALID_INDEX)
        glUniformBlockBinding(target->glID, loc, index);
    else
        exit(0);
}

int main(){
    SDL_Init(SDL_INIT_EVERYTHING);

    const uint32_t WIDTH = 800, HEIGHT = 600;
    const float aspectRaito = (float)WIDTH / HEIGHT;
    const uint32_t FPS_LIMIT = 121;

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
    printf("OpenGL Version: %s\n"
    "Graphics Card: %s\n", glGetString(GL_VERSION), glGetString(GL_RENDERER));
    CMD_Init();
    glEnable(GL_DEPTH_TEST);

    GPI_Buffer ubo = GPI_CreateBuffer(GL_UNIFORM_BUFFER, sizeof(mat4)*2, NULL, GL_DYNAMIC_DRAW);

    vec3 campos = {8.f, 130.0f, 8.f};
    GPI_Camera camera = GPI_CreateCamera(glm_rad(45.f), aspectRaito, campos);

    GPI_Texture tuxTexture = GPI_CreateTexture("res/textures/tux.png", GL_CLAMP_TO_BORDER, GL_NEAREST, GL_TEXTURE_2D);
    GPI_Texture catTexture = GPI_CreateTexture("res/textures/communist-cat.jpg", GL_CLAMP_TO_BORDER, GL_NEAREST, GL_TEXTURE_2D);    

    vec3 eulerCamRotation = {0.f, 0.f, 0.f};

    const float sensitivity = 0.1f;
    const float moveSpeed = 3.f;

    vec3 cp = {0,0,0};
    CMD_Chunk c = CMD_CreateChunk(cp);
    for(uint32_t i = 0; i < CMD_CHUNK_COUNT_ALL; i++)
        c.blocks[i] = &CMD_AirBlock;

    for(uint32_t px = 0; px < CMD_CHUNK_COUNT_X; px++)
    for(uint32_t py = 0; py < CMD_CHUNK_COUNT_Y/2-2; py++)
    for(uint32_t pz = 0; pz < CMD_CHUNK_COUNT_Z; pz++)
    {
        vec3 blockPos = {px, py, pz};
        uint32_t index = CMD_GetParrayOffset(blockPos);
        c.blocks[index] = &CMD_StoneBlock;
    }
    for(uint32_t px = 0; px < CMD_CHUNK_COUNT_X; px++)
    for(uint32_t py = CMD_CHUNK_COUNT_Y/2-2; py < CMD_CHUNK_COUNT_Y/2; py++)
    for(uint32_t pz = 0; pz < CMD_CHUNK_COUNT_Z; pz++)
    {
        vec3 blockPos = {px, py, pz};
        uint32_t index = CMD_GetParrayOffset(blockPos);
        c.blocks[index] = &CMD_GrassBlock;
    }
    CMD_ChunkMesh chunkMesh = CMD_CreateMesh(
        GL_ARRAY_BUFFER, CMD_CHUNK_COUNT_ALL*24*sizeof(CMD_ChunkVertex), GL_DYNAMIC_DRAW,
        GL_ELEMENT_ARRAY_BUFFER, CMD_CHUNK_COUNT_ALL*36*sizeof(uint32_t), GL_DYNAMIC_DRAW, &CMD_ChunkLayout); 
    CMD_RegenerateChunkMesh(&chunkMesh, &c);
    
    CMD_BlockType* blocks[3] = {&CMD_AirBlock, &CMD_StoneBlock, &CMD_GrassBlock};
    uint32_t blocksTop = 0;
    CMD_BlockType* selectedBlock = blocks[blocksTop];

    while(!shouldClose)
    {
        uint32_t lastTime = SDL_GetTicks();

        GPI_SetCameraRotation(&camera, aspectRaito, 
        glm_rad(eulerCamRotation[0]), 
        glm_rad(eulerCamRotation[1]), 
        glm_rad(eulerCamRotation[2]));
        
        vec3 camdx; glm_vec3_scale(camera.forward, moveSpeed * 0.f, camdx);
        GPI_MoveCamera(&camera, camdx);

        mat4 proj; GPI_GetCameraProjection(&camera, aspectRaito, proj);
        mat4 view; GPI_GetCameraView(&camera, view);
        
        GPI_BindBuffer(&ubo);
            glBufferSubData(ubo.TYPE, 0, sizeof(mat4), &proj);
            glBufferSubData(ubo.TYPE, sizeof(mat4), sizeof(mat4), &view);
        GPI_UnbindBuffer(&ubo);
    
        CMD_PollEvents(&input);
        if(input.held[SDL_SCANCODE_ESCAPE])
            shouldClose = 1;
        if(input.held[SDL_SCANCODE_W]) {
            vec3 mv = {camera.forward[0], 0, camera.forward[2]};
            glm_normalize(mv);
            glm_vec3_scale(mv, moveSpeed * windowWrp.deltaTime, mv);
            GPI_MoveCamera(&camera, mv);
        }
        if(input.held[SDL_SCANCODE_S]) {
            vec3 mv = {camera.forward[0], 0, camera.forward[2]};
            glm_normalize(mv);
            glm_vec3_scale(mv, moveSpeed * windowWrp.deltaTime, mv);
            glm_vec3_negate(mv);
            GPI_MoveCamera(&camera, mv);
        }
        if(input.held[SDL_SCANCODE_D]) {
            vec3 mv = {camera.right[0], 0, camera.right[2]};
            glm_normalize(mv);
            glm_vec3_scale(mv, moveSpeed * windowWrp.deltaTime, mv);
            GPI_MoveCamera(&camera, mv);
        }
        if(input.held[SDL_SCANCODE_A]) {
            vec3 mv = {camera.right[0], 0, camera.right[2]};
            glm_normalize(mv);
            glm_vec3_scale(mv, moveSpeed * windowWrp.deltaTime, mv);
            glm_vec3_negate(mv);
            GPI_MoveCamera(&camera, mv);
        }
        if(input.held[SDL_SCANCODE_SPACE]) {
            vec3 mv = {0, moveSpeed * windowWrp.deltaTime, 0};
            GPI_MoveCamera(&camera, mv);
        }
        if(input.held[SDL_SCANCODE_LSHIFT]) {
            vec3 mv = {0, moveSpeed * windowWrp.deltaTime, 0};
            glm_vec3_negate(mv);
            GPI_MoveCamera(&camera, mv);
        }
        if(input.held[SDL_SCANCODE_TAB])
            glDisable(GL_CULL_FACE);
        else
            glEnable(GL_CULL_FACE);

        if(input.mouseButtonPressed == SDL_BUTTON_RIGHT)
        {
            selectedBlock = blocks[blocksTop % 3];
            blocksTop++;
        }
        if(input.mouseButtonPressed == SDL_BUTTON_LEFT)
        {
            vec2 vpPos = {WIDTH/2, HEIGHT/2};
            CMD_SetBlockWorldSpace(&chunkMesh, &c, vpPos, &camera, selectedBlock, CMD_BLOCKPLACEMENT_FUNC);
        }

        eulerCamRotation[0] += -input.deltaMouse[1] * sensitivity;
        eulerCamRotation[1] += -input.deltaMouse[0] * sensitivity;
        //rendering
        GPI_BindShader(&CMD_ChunkShader);
        int32_t samplers[32];
        for(uint32_t i = 0; i < 32; ++i)
            samplers[i] = i;
        int32_t loc = GPI_GetUniformLocation(&CMD_ChunkShader, "u_Textures");
        if(loc != -1)
            glUniform1iv(loc, 32, samplers);
        glBindTextureUnit(0, CMD_TextureAtlases[0].glID);
        glBindTextureUnit(1, tuxTexture.glID);

        const uint8_t index = 0;
        glBindBufferRange(ubo.TYPE, index, ubo.glID, 0, sizeof(mat4)*2);

        GPI_SetUniformBlock(&CMD_ChunkShader, "ProjectionView", index);
        
        mat4 model; glm_mat4_identity(model);
        loc = GPI_GetUniformLocation(&CMD_ChunkShader, "u_Model");
        if(loc != -1)
            glUniformMatrix4fv(loc, 1, GL_FALSE, model);

        glClearColor(0.7,1,1,1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        CMD_RenderChunkMesh(&chunkMesh, c.position);

        SDL_GL_SwapWindow(window);

        uint32_t currentTime = SDL_GetTicks();    
        uint32_t tickDiff = currentTime - lastTime;
        if((tickDiff) < (1000 / FPS_LIMIT))
            SDL_Delay((1000 / FPS_LIMIT) - (tickDiff));
        windowWrp.deltaTime = (float)(SDL_GetTicks() - lastTime) / 1000.f;   
        
        char wintitle[256];
        sprintf(wintitle, "dt: %f", windowWrp.deltaTime);
        SDL_SetWindowTitle(window, wintitle);
    }

    SDL_DestroyWindow(window);
    CMD_Quit();
    SDL_Quit();
}