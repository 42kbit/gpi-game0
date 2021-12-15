#ifndef __HEADER_CMD_WINDOW
#define __HEADER_CMD_WINDOW

#include "SDL.h"

typedef struct 
{
    SDL_Window* windowPtr;
    float deltaTime;
} CMD_Window;

CMD_Window CMD_CreateWindow(SDL_Window* windowPtr);

#endif /* __HEADER_CMD_WINDOW */
