#ifndef __HEADER_CMD_INPUT
#define __HEADER_CMD_INPUT

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

#endif /* __HEADER_CMD_INPUT */
