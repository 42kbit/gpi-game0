#ifndef _HEADER__CMD_WINDOW
#define _HEADER__CMD_WINDOW

#include "SDL.h"

struct CMD_Window
{
    SDL_Window* windowPtr = NULL;
    float deltaTime = 0.f;
};

CMD_Window CMD_CreateWindow(SDL_Window* windowPtr);

#endif /* _HEADER__CMD_WINDOW */
