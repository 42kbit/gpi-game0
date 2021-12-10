#ifndef _HEADER__CMD_INPUT
#define _HEADER__CMD_INPUT

/*
    SDL_Event wrapper
    stores mouseDelta
           keyInputs

*/

#include "SDL.h"
#include "vec2.h"

#include <inttypes.h>

typedef struct
{
    SDL_Event event;
    vec2 deltaMouse;
    int8_t pressed[SDL_NUM_SCANCODES];
} CMD_Input;

CMD_Input CMD_CreateInput();
void CMD_PollEvents(CMD_Input* target);

#endif /* _HEADER__CMD_INPUT */
