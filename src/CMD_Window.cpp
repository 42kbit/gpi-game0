#include "CMD_Window.h"

CMD_Window CMD_CreateWindow(SDL_Window* windowPtr)
{
    return {windowPtr, 0.f};
}