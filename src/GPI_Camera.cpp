#include "GPI_Camera.h"

GPI_Camera GPI_CreateCamera(float radFov, float aspectRaito, glm::vec3 position)
{
    return {
        radFov, 
        glm::mat4(1.f), 
        position, 
        glm::vec3(0), 
        glm::vec3(0), 
        glm::vec3(0)
    };
}

void GPI_SetCameraRotation(GPI_Camera* target, float aspectRaito, float x, float y, float z)
{
	target->rotation = glm::mat4(1);

    target->rotation = glm::rotate(target->rotation, z, glm::vec3(0, 0, 1));
	target->rotation = glm::rotate(target->rotation, y, glm::vec3(0, 1, 0));
	target->rotation = glm::rotate(target->rotation, x, glm::vec3(1, 0, 0));

    GPI_UpdateCameraDirectionalVectors(target);
}

void GPI_SetCameraRotationWin(GPI_Camera* target, SDL_Window* window, float x, float y, float z)
{
    int32_t wx,wy;
    SDL_GetWindowSize(window, &wx, &wy);
    GPI_SetCameraRotation(target, (float)wx/(float)wy, x,y,z);
}

void GPI_MoveCamera(GPI_Camera* target, const glm::vec3& position)
{
	target->position += position;
}

void GPI_UpdateCameraDirectionalVectors(GPI_Camera* target){
	target->forward = glm::vec3(target->rotation * glm::vec4(0, 0, -1, 1));
	target->right =   glm::vec3(target->rotation * glm::vec4(1, 0,  0, 1));
	target->up =      glm::vec3(target->rotation * glm::vec4(0, 1,  0, 1));
}

glm::mat4 GPI_GetCameraView(GPI_Camera* target)
{
    return glm::lookAt(target->position, target->position + target->forward, target->up);
}

glm::mat4 GPI_GetCameraProjection(GPI_Camera* target, float aspectRaito){
    return glm::perspective(target->fov, aspectRaito, 0.1f, 100.f);

}