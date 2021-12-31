#ifndef __HEADER_CMD_BLOCKTYPE
#define __HEADER_CMD_BLOCKTYPE

#include <inttypes.h>

#define CMD_BLOCK_TRANSPARENCY_MASK 0b00000001
#define CMD_BLOCK_NEGATION_MASK 0b00000010

typedef struct {
    uint32_t blockID;
    uint8_t flags;
    // flags, other stuff 
} CMD_BlockType;

#endif /* __HEADER_CMD_BLOCKTYPE */
