#include "GPI_Camera.h"
#include "cam.h"
#include "affine.h"

GPI_Camera GPI_CreateCamera(float radFov, float aspectRaito, vec3 pos)
{
    GPI_Camera cam;
    cam.fov = radFov;
    cam.position[0] = pos[0];
    cam.position[1] = pos[1];
    cam.position[2] = pos[2];
    return cam;
}

void GPI_MoveCamera(GPI_Camera* target, vec3 dv) 
{
    glm_vec3_add(target->position, dv, target->position);
}

void GPI_SetCameraRotation(GPI_Camera* target, float aspectRaito, float x, float y, float z) 
{
    glm_mat4_identity(target->rotation);

    vec3 ox = {1, 0, 0};
    vec3 oy = {0, 1, 0};
    vec3 oz = {0, 0, 1};

    glm_rotate(target->rotation, z, oz);
    glm_rotate(target->rotation, y, oy);
    glm_rotate(target->rotation, x, ox);

    GPI_UpdateCameraDirectionalVectors(target);
}

void GPI_SetCameraRotationWin(GPI_Camera* target, SDL_Window* window, float x, float y, float z)
{
    int32_t wx, wy;
    SDL_GetWindowSize(window, &wx, &wy);
    GPI_SetCameraRotation(target, (float)wx/(float)wy, x, y, z); 
}

void GPI_UpdateCameraDirectionalVectors(GPI_Camera* target)
{
    vec4 fw = {0, 0, -1, 1};
    vec4 rg = {1, 0,  0, 1};
    vec4 up = {0, 1,  0, 1};
    vec4 fres, rres, ures;

    glm_mat4_mulv(target->rotation, fw, fres);
    glm_mat4_mulv(target->rotation, rg, rres);
    glm_mat4_mulv(target->rotation, up, ures);
    glm_vec3(fres, target->forward);
    glm_vec3(rres, target->right);
    glm_vec3(ures, target->up);
}

void GPI_GetCameraView(GPI_Camera* target, mat4* outView)
{
    vec3 v; glm_vec3_add(target->position, target->forward, v);
    glm_lookat(target->position, v, target->up, outView);
}

void GPI_GetCameraProjection(GPI_Camera* target, float aspectRaito, mat4* outProj)
{
    glm_perspective(target->fov, aspectRaito, 0.1, 1000, outProj);
}