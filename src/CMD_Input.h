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
    glm::vec2 deltaMouse;
    bool pressed[512];
};

CMD_Input CMD_CreateInput();
void CMD_PollEvents(CMD_Input* target, SDL_Event* sdlEvent);

#endif /* _HEADER__CMD_INPUT */
