#ifndef _HEADER__GPI_CAMERA
#define _HEADER__GPI_CAMERA
#include "glm/gtc/matrix_transform.hpp"
#include "glm/vec3.hpp"
#include "SDL.h"

struct GPI_Camera
{
    float fov;

    glm::mat4 rotation;
    glm::vec3 position;

    glm::vec3 forward;
    glm::vec3 up;
    glm::vec3 right;
};

GPI_Camera GPI_CreateCamera(float radFov, float aspectRaito, glm::vec3 position);
void GPI_MoveCamera(GPI_Camera* target, const glm::vec3& position);
void GPI_SetCameraRotation(GPI_Camera* target, float aspectRaito, float x, float y, float z);
void GPI_SetCameraRotationWin(GPI_Camera* target, SDL_Window* window, float x, float y, float z);
void GPI_UpdateCameraDirectionalVectors(GPI_Camera* target);
glm::mat4 GPI_GetCameraView(GPI_Camera* target);
glm::mat4 GPI_GetCameraProjection(GPI_Camera* target, float aspectRaito);

#endif /* _HEADER__GPI_CAMERA */
