#include "CMD_Input.h"

CMD_Input CMD_CreateInput()
{
    CMD_Input input;
    memset(input.held, 0, sizeof(input.held));
    memset(input.pressed, 0, sizeof(input.pressed));
    return input;
}

void CMD_PollEvents(CMD_Input* target)
{
    CMD_ResetEvents(target);
    while(SDL_PollEvent(&target->event))
    {
        switch (target->event.type)
        {
        case SDL_KEYDOWN:
        {
            target->held[target->event.key.keysym.scancode] = 1;
            target->pressed[target->event.key.keysym.scancode] = 0;
            break;
        }
        case SDL_KEYUP:
        {
            target->pressed[target->event.key.keysym.scancode] = 1;
            target->held[target->event.key.keysym.scancode] = 0;
            break;
        }
        case SDL_MOUSEMOTION:
        {
            target->deltaMouse[0] += target->event.motion.xrel;
            target->deltaMouse[1] += target->event.motion.yrel;
            break;
        }
        case SDL_MOUSEBUTTONDOWN:
        {
            target->mouseButtonPressed = target->event.button.button;
        }
        }
    }
}
void CMD_ResetEvents(CMD_Input* target)
{
    memset(target->pressed, 0, sizeof(target->pressed));
    memset(target->deltaMouse, 0, sizeof(target->deltaMouse));
    memset(&target->mouseButtonPressed, 0, sizeof(target->mouseButtonPressed));
}

int8_t CMD_GetKeyPressed(CMD_Input* target, uint32_t keyCode)
{
    int8_t state = target->pressed[keyCode];
    target->pressed[keyCode] = 0;
    return state;
}