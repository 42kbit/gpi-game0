#ifndef _HEADER__GPI_SHADERCOMPILER
#define _HEADER__GPI_SHADERCOMPILER

#include <inttypes.h>

uint32_t GPI_CreateShader(const char* src, uint32_t TYPE);

uint32_t GPI_CreateProgram(uint32_t vertexShaderID, uint32_t fragmentShaderID);

#endif /* _HEADER__GPI_SHADERCOMPILER */
