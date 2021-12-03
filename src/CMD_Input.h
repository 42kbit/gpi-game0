#ifndef _HEADER__CMD_INPUT
#define _HEADER__CMD_INPUT

/*
    SDL_Event wrapper
    stores mouseDelta
           keyInputs

*/

#include "SDL.h"
#include "glm/vec2.hpp"

struct CMD_Input
{
    SDL_Event event;
    glm::vec2 deltaMouse;
    bool pressed[SDL_NUM_SCANCODES];
};

CMD_Input CMD_CreateInput();
void CMD_PollEvents(CMD_Input* target);

#endif /* _HEADER__CMD_INPUT */
