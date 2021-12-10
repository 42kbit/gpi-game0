#ifndef _HEADER__CMD_WINDOW
#define _HEADER__CMD_WINDOW

#include "SDL.h"

typedef struct 
{
    SDL_Window* windowPtr;
    float deltaTime;
} CMD_Window;

CMD_Window CMD_CreateWindow(SDL_Window* windowPtr);

#endif /* _HEADER__CMD_WINDOW */
