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

#include "CMD_ChunkData.h"

#include "CMD_Global.h"
#include "CMD_Meshes.h"

#include "CMD_ChunkRenderer.h"

#include "project.h"

#include "CMD_World.h"

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

    vec3 campos = {8.f, 130.0f, 8.f};
    GPI_Camera camera = GPI_CreateCamera(glm_rad(45.f), aspectRaito, campos);

    GPI_Texture tuxTexture = GPI_CreateTexture("res/textures/tux.png", GL_CLAMP_TO_BORDER, GL_NEAREST, GL_TEXTURE_2D);
    GPI_Texture catTexture = GPI_CreateTexture("res/textures/communist-cat.jpg", GL_CLAMP_TO_BORDER, GL_NEAREST, GL_TEXTURE_2D);    

    vec3 eulerCamRotation = {0.f, 0.f, 0.f};

    const float sensitivity = 0.1f;
    const float moveSpeed = 3.f;
    
    CMD_BlockType* blocks[3] = {&CMD_AirBlock, &CMD_StoneBlock, &CMD_GrassBlock};
    uint32_t blocksTop = 0;
    CMD_BlockType* selectedBlock = blocks[blocksTop];
    
    uint32_t cpos[3] = {0,0,0};
    CMD_ChunkData basicTerrain = CMD_CreateChunkData(cpos);
    for(uint32_t i = 0; i < CMD_CHUNK_COUNT_ALL; i++)
        basicTerrain.blocks[i] = &CMD_AirBlock;
    for(uint32_t x = 0; x < CMD_CHUNK_COUNT_X; x++)
        for(uint32_t y = 0; y < CMD_CHUNK_COUNT_Y / 2; y++)
            for(uint32_t z = 0; z < CMD_CHUNK_COUNT_Z; z++)
            {
                vec3 pos = {x,y,z};
                basicTerrain.blocks[CMD_GetParrayOffset(pos)] = &CMD_GrassBlock;
            }

    CMD_World world; CMD_CreateWorld(&world);
    CMD_Chunk* chunk;
    CMD_AllocateEmptyChunk(&world, &chunk);
    CMD_PushChunkData(chunk, &basicTerrain);
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
        
        GPI_BindBuffer(&CMD_ProjectionViewUniformBlock);
            glBufferSubData(CMD_ProjectionViewUniformBlock.TYPE, 0, sizeof(mat4), &proj);
            glBufferSubData(CMD_ProjectionViewUniformBlock.TYPE, sizeof(mat4), sizeof(mat4), &view);
        GPI_UnbindBuffer(&CMD_ProjectionViewUniformBlock);
    
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
            CMD_SetBlockWorldSpace(&world.loadedChunks[0].chunkMesh, &world.loadedChunks[0].chunkData, vpPos, &camera, selectedBlock, CMD_BLOCKPLACEMENT_FUNC);
        }

        eulerCamRotation[0] += -input.deltaMouse[1] * sensitivity;
        eulerCamRotation[1] += -input.deltaMouse[0] * sensitivity;

        glClearColor(0.7,1,1,1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        CMD_RenderAllChunks(&world);

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
