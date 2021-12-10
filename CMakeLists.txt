cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

set(PROJECT_NAME gpi) #GRAPHICS PROGRAMMING INTERFACE

project(${PROJECT_NAME})

add_library(${PROJECT_NAME} 
src/GPI_FileReader.c
src/GPI_ShaderCompiler.c
src/GPI_TextureReader.c

src/GPI_Buffer.c
src/GPI_Shader.c
src/GPI_Texture.c
src/GPI_VertexLayout.c
src/GPI_Renderer.c
src/GPI_VertexArray.c

src/GPI_Camera.c
)

add_subdirectory(external/glad)
target_link_libraries(${PROJECT_NAME} glad)

add_subdirectory(external/SDL2)
target_link_libraries(${PROJECT_NAME} SDL2)

add_subdirectory(external/cglm)

target_include_directories(${PROJECT_NAME} PUBLIC src/)
target_include_directories(${PROJECT_NAME} PUBLIC external/)
target_include_directories(${PROJECT_NAME} PUBLIC external/cglm/include/cglm)
target_include_directories(${PROJECT_NAME} PUBLIC external/SDL2/include)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)