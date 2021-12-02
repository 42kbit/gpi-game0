#define SDL_MAIN_HANDLED
#include "SDL.h"
#include "glad/glad.h"

#include "GlWrappers/GPI_Buffer.h"
#include "GlWrappers/GPI_Shader.h"

#include "GlWrappers/GPI_Texture.h"

#include "GlWrappers/GPI_Renderer.h"
#include "GlWrappers/GPI_VertexArray.h"

#include <stddef.h>
#include <iostream>

#include "Engine/GPI_Camera.h"

struct Vertex
{
    glm::vec3 position;
    glm::vec2 textureCoords;
    float texIndex;
};

const uint32_t WIDTH = 800, HEIGHT = 600;
const float aspectRaito = (float)WIDTH / HEIGHT;
const uint32_t FPS_LIMIT = 120;

int main(){
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window* window = SDL_CreateWindow(
        "SDL window",
        SDL_WINDOWPOS_CENTERED, 
        SDL_WINDOWPOS_CENTERED, 
        WIDTH, 
        HEIGHT, 
        SDL_WINDOW_OPENGL);
    SDL_Event ev;
    int8_t shouldClose = 0;

    SDL_SetRelativeMouseMode(SDL_TRUE);

    SDL_GLContext context = SDL_GL_CreateContext(window);
    gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);
    gladLoadGL();

    Vertex vertecies[4];
    vertecies[0] = {{0.f, 0.f, 0.f}, {0.f, 0.f}, 0.f};
    vertecies[1] = {{1.f, 0.f, 0.f}, {1.f, 0.f}, 0.f};
    vertecies[2] = {{1.f, 1.f, 0.f}, {1.f, 1.f}, 0.f};
    vertecies[3] = {{0.f, 1.f, 0.f}, {0.f, 1.f}, 0.f};
    uint32_t indecies[6] = {0,1,2, 2,3,0};

    GPI_Buffer vbo = GPI_CreateBuffer(GL_ARRAY_BUFFER, sizeof(vertecies), vertecies, GL_STATIC_DRAW);
    GPI_Buffer ibo = GPI_CreateBuffer(GL_ELEMENT_ARRAY_BUFFER, sizeof(indecies), indecies, GL_STATIC_DRAW);
    GPI_Buffer ubo = GPI_CreateBuffer(GL_UNIFORM_BUFFER, sizeof(glm::mat4)*2, nullptr, GL_DYNAMIC_DRAW);

    GPI_Shader shaderProgram = GPI_CreateShaderFromFiles("res/shaders/defaultVertex.glsl", "res/shaders/textureDefault.glsl");
    GPI_Texture tuxTexture = GPI_CreateTexture("res/textures/tux.png");
    GPI_Camera camera = GPI_CreateCamera(glm::radians(45.f), aspectRaito, glm::vec3(0.f, 0.f, 3.f));

    int32_t loc = GPI_GetUniformLocation(&shaderProgram, "u_Textures");
    int32_t textureIndecies[32];
    for(uint32_t i = 0; i < 32; i++)
        textureIndecies[i] = i;
    if(loc != -1)
        glUniform1iv(loc, sizeof(textureIndecies) / sizeof(textureIndecies[0]), textureIndecies);
    glBindTextureUnit(0, tuxTexture.glID);

    GPI_VertexLayout layout;
    GPI_PushLayoutData(&layout, 3, GL_FLOAT, GL_FALSE);
    GPI_PushLayoutData(&layout, 2, GL_FLOAT, GL_FALSE);
    GPI_PushLayoutData(&layout, 1, GL_FLOAT, GL_FALSE);
    
    GPI_VertexArray vao = GPI_CreateVertexArray(&layout, &vbo, &ibo);
    GPI_BindVertexArrayAttribs(&vao);
    GPI_UnbindVertexArray(&vao);

    float deltaTime = 0.0f;
    
    glm::vec3 eulerCamRotation = {0.f, 0.f, 0.f};
    
    glm::vec2 inputDir = {0.f, 0.f};
    glm::vec2 deltaMouse = {0.f, 0.f};

    const float sensitivity = 0.01f;
    const float moveSpeed = 0.01f;

    while(!shouldClose)
    {
        uint32_t lastTime = SDL_GetTicks();

        GPI_SetCameraRotation(&camera, aspectRaito, 
        glm::radians(eulerCamRotation.x), 
        glm::radians(eulerCamRotation.y), 
        glm::radians(eulerCamRotation.z));
        
        GPI_MoveCamera(&camera, camera.forward * moveSpeed * inputDir.y);

        inputDir = {0.f, 0.f};
        deltaMouse = {0.f, 0.f};

        glm::mat4 proj = GPI_GetCameraProjection(&camera, aspectRaito);
        glm::mat4 view = GPI_GetCameraView(&camera);

        GPI_BindBuffer(&ubo);
            glBufferSubData(ubo.TYPE, 0, sizeof(glm::mat4), &proj);
            glBufferSubData(ubo.TYPE, sizeof(glm::mat4), sizeof(glm::mat4), &view);
        GPI_UnbindBuffer(&ubo);

        while(SDL_PollEvent(&ev))
        {
            switch (ev.type)
            {
            case SDL_QUIT:
                shouldClose = 1;
                break;
            
            case SDL_KEYDOWN:
            {
                switch(ev.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                    {
                        shouldClose = 1;
                        break;
                    }
                }
                break;
            }
            case SDL_MOUSEMOTION:
            {
                deltaMouse.x += ev.motion.xrel;
                deltaMouse.y += ev.motion.yrel;
                break;
            }
            }
        }

        eulerCamRotation.x += -deltaMouse.y * sensitivity;
        eulerCamRotation.y += -deltaMouse.x * sensitivity;

        //rendering
        glClearColor(0,0,0,1);
        glClear(GL_COLOR_BUFFER_BIT);

        const uint8_t index = 0;
        glBindBufferRange(GL_UNIFORM_BUFFER, index, ubo.glID, 0, sizeof(glm::mat4)*2);

        uint32_t blockLoc = glGetUniformBlockIndex(shaderProgram.glID, "ProjectonView");
        if(blockLoc != GL_INVALID_INDEX)
            glUniformBlockBinding(shaderProgram.glID, blockLoc, index);
        
        glm::mat4 model = glm::mat4(1);
        loc = GPI_GetUniformLocation(&shaderProgram, "u_Model");
        if(loc != -1)
            glUniformMatrix4fv(loc, 1, GL_FALSE, &model[0][0]);

        GPI_BindShader(&shaderProgram);

        GPI_BindVertexArray(&vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);

        SDL_GL_SwapWindow(window);

        uint32_t currentTime = SDL_GetTicks();    
        uint32_t tickDiff = currentTime - lastTime;
        if((tickDiff) < (1000 / FPS_LIMIT))
            SDL_Delay((1000 / FPS_LIMIT) - (tickDiff));
        deltaTime = (float)(SDL_GetTicks() - lastTime) / 1000.f;
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
}
