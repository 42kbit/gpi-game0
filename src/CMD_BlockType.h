#ifndef __HEADER_CMD_BLOCKTYPE
#define __HEADER_CMD_BLOCKTYPE
#include <inttypes.h>

#define CMD_BLOCKID_AIR 0
#define CMD_BLOCKID_GRASS 1

typedef struct {
    uint32_t blockID;
    uint8_t isTransparent;
    // flags, other stuff
} CMD_BlockType;

#endif /* __HEADER_CMD_BLOCKTYPE */
