#include "CMD_Input.h"

CMD_Input CMD_CreateInput()
{
    CMD_Input input;
    memset(input.pressed, 0, sizeof(input.pressed));
    return input;
}

void CMD_PollEvents(CMD_Input* target, SDL_Event* sdlEvent)
{
    while(SDL_PollEvent(sdlEvent))
    {
        switch (sdlEvent->type)
        {
        case SDL_KEYDOWN:
        {
            target->pressed[sdlEvent->key.keysym.sym] = true;
            break;
        }
        case SDL_KEYUP:
        {
            target->pressed[sdlEvent->key.keysym.sym] = false;
            break;
        }
        case SDL_MOUSEMOTION:
        {
            target->deltaMouse.x += sdlEvent->motion.xrel;
            target->deltaMouse.y += sdlEvent->motion.yrel;
            break;
        }
        }
    }
}