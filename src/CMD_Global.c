#include "CMD_Global.h"

const uint8_t CMD_CHUNK_RENDER_DISTANCE = 0;

const uint32_t CMD_PROJECTION_VIEW_UNIFORMBLOCK_INDEX = 0;

const CMD_BlockType CMD_AirBlock = {CMD_BLOCKID_AIR, 1};
const CMD_BlockType CMD_GrassBlock = {CMD_BLOCKID_GRASS, 0};

const uint8_t CMD_VOXEL_VERTECIES[] = 
{ 
    //+x
    1,0,0,0, //TODO: add texture id 
    1,1,0,1,
    1,1,1,2,
    1,0,1,3,    
    //-x
    0,0,1,0,
    0,1,1,1,
    0,1,0,2,
    0,0,0,3,
    //+y
    1,1,1,2,
    1,1,0,3,
    0,1,0,0,
    0,1,1,1,
    //-y
    1,0,0,3,
    1,0,1,2,
    0,0,1,1,
    0,0,0,0,
    //+z
    1,0,1,0,
    1,1,1,1,
    0,1,1,2,
    0,0,1,3,
    //-z
    0,0,0,0,
    0,1,0,1,
    1,1,0,2,
    1,0,0,3
};