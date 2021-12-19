#ifndef __HEADER_CMD_GLOBAL
#define __HEADER_CMD_GLOBAL

#include <inttypes.h>
#include "CMD_BlockType.h"

const uint32_t CMD_PROJECTION_VIEW_UNIFORMBLOCK_INDEX = 0;

const CMD_BlockType CMD_AirBlock = {CMD_BLOCKID_AIR};
const CMD_BlockType CMD_GrassBlock = {CMD_BLOCKID_GRASS};

const uint8_t CMD_VOXEL_VERTECIES[] = 
{
    //-z
    0,0,0,0,
    0,1,0,1,
    1,1,0,2,
    1,0,0,3,
    //+x
    1,0,0,0,
    1,1,0,1,
    1,1,1,2,
    1,0,1,3,
    //+z
    1,0,1,0,
    1,1,1,1,
    0,1,1,2,
    0,0,1,3,
    //-x
    0,0,1,0,
    0,1,1,1,
    0,1,0,2,
    0,0,0,3,
    //-y
    1,0,0,3,
    1,0,1,2,
    0,0,1,1,
    0,0,0,0,
    //+y
    1,1,1,2,
    1,1,0,3,
    0,1,0,0,
    0,1,1,1
};

#endif /* __HEADER_CMD_GLOBAL */
