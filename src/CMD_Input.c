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
            target->pressed[target->event.key.keysym.scancode] = true;
            break;
        }
        case SDL_KEYUP:
        {
            target->pressed[target->event.key.keysym.scancode] = false;
            break;
        }
        case SDL_MOUSEMOTION:
        {
            target->deltaMouse[0] += target->event.motion.xrel;
            target->deltaMouse[1] += target->event.motion.yrel;
            break;
        }
        }
    }
}