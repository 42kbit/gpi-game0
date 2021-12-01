#ifndef H_SHADERCOMPILER
#define H_SHADERCOMPILER

#include <cinttypes>

uint32_t GPI_CreateShader(const char* src, uint32_t TYPE);

uint32_t GPI_CreateProgram(uint32_t vertexShaderID, uint32_t fragmentShaderID);

#endif // H_SHADERCOMPILER
