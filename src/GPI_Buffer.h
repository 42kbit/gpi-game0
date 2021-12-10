#ifndef _HEADER__GPI_BUFFER
#define _HEADER__GPI_BUFFER

#include <inttypes.h>

typedef struct
{
    uint32_t glID;
    uint32_t TYPE;
    uint32_t DRAW_MODE;
} GPI_Buffer;

GPI_Buffer GPI_CreateBuffer(uint32_t TYPE,
                    uint32_t byte_size,
                    void* data,
                    uint32_t DRAW_MODE);

void GPI_BindBuffer(GPI_Buffer* buffer);
void GPI_UnbindBuffer(GPI_Buffer* buffer);

#endif /* _HEADER__GPI_BUFFER */
