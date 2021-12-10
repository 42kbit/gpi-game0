#ifndef _HEADER__GPI_CAMERA
#define _HEADER__GPI_CAMERA
#include "struct/vec3.h"
#include "struct/mat4.h"
#include "SDL.h"

typedef struct
{
    float fov;

    mat4 rotation;
    vec3 position;

    vec3 forward;
    vec3 up;
    vec3 right;
} GPI_Camera;

GPI_Camera GPI_CreateCamera(float radFov, float aspectRaito, vec3 position);
void GPI_MoveCamera(GPI_Camera* target, vec3 position);
void GPI_SetCameraRotation(GPI_Camera* target, float aspectRaito, float x, float y, float z);
void GPI_SetCameraRotationWin(GPI_Camera* target, SDL_Window* window, float x, float y, float z);
void GPI_UpdateCameraDirectionalVectors(GPI_Camera* target);
void GPI_GetCameraView(GPI_Camera* target, mat4* outView);
void GPI_GetCameraProjection(GPI_Camera* target, float aspectRaito, mat4* outProj);

#endif /* _HEADER__GPI_CAMERA */
