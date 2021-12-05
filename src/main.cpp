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
#include <iostream>

#include "CMD_Input.h"
#include "CMD_Window.h"

#include "CMD_VertexTypes.h"
#include "CMD_BatchRenderer.h"

#include "CMD_Global.h"

const uint32_t WIDTH = 800, HEIGHT = 600;
const float aspectRaito = (float)WIDTH / HEIGHT;
const uint32_t FPS_LIMIT = 120;

// TODO: Add camera wrapper

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

    GPI_Buffer ubo = GPI_CreateBuffer(GL_UNIFORM_BUFFER, sizeof(glm::mat4)*2, nullptr, GL_DYNAMIC_DRAW);

    GPI_Shader shaderProgram = GPI_CreateShaderFromFiles("res/shaders/defaultVertex.glsl", "res/shaders/textureDefault.glsl");
    GPI_Texture tuxTexture = GPI_CreateTexture("res/textures/tux.png", GL_CLAMP_TO_BORDER, GL_NEAREST);
    GPI_Camera camera = GPI_CreateCamera(glm::radians(45.f), aspectRaito, glm::vec3(0.f, 0.f, 3.f));

    int32_t loc = GPI_GetUniformLocation(&shaderProgram, "u_Textures");
    int32_t textureIndecies[32];
    for(uint32_t i = 0; i < 32; i++)
        textureIndecies[i] = i;
    if(loc != -1)
        glUniform1iv(loc, sizeof(textureIndecies) / sizeof(textureIndecies[0]), textureIndecies);
    glBindTextureUnit(0, tuxTexture.glID);
    
    glm::vec3 eulerCamRotation = {0.f, 0.f, 0.f};

    const float sensitivity = 0.1f;
    const float moveSpeed = 1.f;

    CMD_BatchData data = CMD_CreateBatchData(&shaderProgram);

    while(!shouldClose)
    {
        uint32_t lastTime = SDL_GetTicks();

        GPI_SetCameraRotation(&camera, aspectRaito, 
        glm::radians(eulerCamRotation.x), 
        glm::radians(eulerCamRotation.y), 
        glm::radians(eulerCamRotation.z));
        
        GPI_MoveCamera(&camera, camera.forward * moveSpeed * 0.f);

        input.deltaMouse = {0.f, 0.f};

        glm::mat4 proj = GPI_GetCameraProjection(&camera, aspectRaito);
        glm::mat4 view = GPI_GetCameraView(&camera);

        GPI_BindBuffer(&ubo);
            glBufferSubData(ubo.TYPE, 0, sizeof(glm::mat4), &proj);
            glBufferSubData(ubo.TYPE, sizeof(glm::mat4), sizeof(glm::mat4), &view);
        GPI_UnbindBuffer(&ubo);

        CMD_PollEvents(&input);
        if(input.pressed[SDL_SCANCODE_ESCAPE])
            shouldClose = 1;
        if(input.pressed[SDL_SCANCODE_W]) {
            glm::vec3 mv = glm::vec3(camera.forward.x, 0, camera.forward.z);
            GPI_MoveCamera(&camera, mv * moveSpeed * windowWrp.deltaTime);
        }
        if(input.pressed[SDL_SCANCODE_S]) {
            glm::vec3 mv = glm::vec3(camera.forward.x, 0, camera.forward.z);
            GPI_MoveCamera(&camera, -mv * moveSpeed * windowWrp.deltaTime);
        }
        if(input.pressed[SDL_SCANCODE_D]) {
            glm::vec3 mv = glm::vec3(camera.right.x, 0, camera.right.z);
            GPI_MoveCamera(&camera, mv * moveSpeed * windowWrp.deltaTime);
        }
        if(input.pressed[SDL_SCANCODE_A]) {
            glm::vec3 mv = glm::vec3(camera.right.x, 0, camera.right.z);
            GPI_MoveCamera(&camera, -mv * moveSpeed * windowWrp.deltaTime);
        }
        if(input.pressed[SDL_SCANCODE_SPACE]) {
            GPI_MoveCamera(&camera, glm::vec3(0,moveSpeed,0) * windowWrp.deltaTime);
        }
        if(input.pressed[SDL_SCANCODE_LSHIFT]) {
            GPI_MoveCamera(&camera, glm::vec3(0,-moveSpeed,0) * windowWrp.deltaTime);
        }

        eulerCamRotation.x += -input.deltaMouse.y * sensitivity;
        eulerCamRotation.y += -input.deltaMouse.x * sensitivity;

        //rendering
        glClearColor(0,0,0,1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        const uint8_t index = 0;
        glBindBufferRange(GL_UNIFORM_BUFFER, index, ubo.glID, 0, sizeof(glm::mat4)*2);

        GPI_SetUniformBlock(&shaderProgram, "ProjectionView", index);
        
        glm::mat4 model = glm::mat4(1);
        loc = GPI_GetUniformLocation(&shaderProgram, "u_Model");
        if(loc != -1)
            glUniformMatrix4fv(loc, 1, GL_FALSE, &model[0][0]);

        CMD_BeginBatch(&data);

        CMD_PushQuadData(&data, {0,0,1});
        CMD_PushQuadData(&data, {0,1,-1});
        CMD_PushQuadData(&data, {1,0,0});
        
        CMD_EndBatch(&data);
        CMD_Flush(&data);

        SDL_GL_SwapWindow(window);

        uint32_t currentTime = SDL_GetTicks();    
        uint32_t tickDiff = currentTime - lastTime;
        if((tickDiff) < (1000 / FPS_LIMIT))
            SDL_Delay((1000 / FPS_LIMIT) - (tickDiff));
        windowWrp.deltaTime = (float)(SDL_GetTicks() - lastTime) / 1000.f;
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
}
