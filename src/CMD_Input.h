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
    int8_t held[SDL_NUM_SCANCODES];
    int8_t pressed[SDL_NUM_SCANCODES]; // only function usage!
} CMD_Input;

CMD_Input CMD_CreateInput();
void CMD_PollEvents(CMD_Input* target);

int8_t CMD_GetPressed(CMD_Input* target, uint32_t keyCode);

#endif /* __HEADER_CMD_INPUT */
