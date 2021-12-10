#include "CMD_Window.h"

CMD_Window CMD_CreateWindow(SDL_Window* windowPtr)
{
    CMD_Window win;
    memset(&win, 0, sizeof(win));
    return win;
}