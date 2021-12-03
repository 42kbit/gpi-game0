#include "CMD_Input.h"

CMD_Input CMD_CreateInput()
{
    CMD_Input input;
    memset(input.pressed, 0, sizeof(input.pressed));
    return input;
}

void CMD_PollEvents(CMD_Input* target)
{
    while(SDL_PollEvent(&target->event))
    {
        switch (target->event.type)
        {
        case SDL_KEYDOWN:
        {
            target->pressed[target->event.key.keysym.sym] = true;
            break;
        }
        case SDL_KEYUP:
        {
            target->pressed[target->event.key.keysym.sym] = false;
            break;
        }
        case SDL_MOUSEMOTION:
        {
            target->deltaMouse.x += target->event.motion.xrel;
            target->deltaMouse.y += target->event.motion.yrel;
            break;
        }
        }
    }
}