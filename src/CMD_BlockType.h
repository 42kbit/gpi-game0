#ifndef __HEADER_CMD_BLOCKTYPE
#define __HEADER_CMD_BLOCKTYPE
#include <inttypes.h>

#define CMD_BLOCKID_AIR 0
#define CMD_BLOCKID_GRASS 1

typedef struct {
    uint32_t blockID;
    uint32_t atlasOrientation; // 8 bits for x,y pos, 
    // flags, other stuff
} CMD_BlockType;

#endif /* __HEADER_CMD_BLOCKTYPE */
