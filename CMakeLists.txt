cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

set(PROJECT_NAME gpi) #GRAPHICS PROGRAMMING INTERFACE

project(${PROJECT_NAME})

add_library(${PROJECT_NAME} 
src/Modules/GPI_FileReader.cpp
src/Modules/GPI_ShaderCompiler.cpp
src/Modules/GPI_TextureReader.cpp

src/GlWrappers/GPI_Buffer.cpp 
src/GlWrappers/GPI_Shader.cpp
src/GlWrappers/GPI_Texture.cpp
src/GlWrappers/GPI_VertexLayout.cpp
src/GlWrappers/GPI_Renderer.cpp
src/GlWrappers/GPI_VertexArray.cpp

src/Engine/GPI_Camera.cpp
)

add_subdirectory(external/glad)
target_link_libraries(${PROJECT_NAME} glad)

add_subdirectory(external/SDL2)
target_link_libraries(${PROJECT_NAME} SDL2)

add_subdirectory(external/glm)

target_include_directories(${PROJECT_NAME} PUBLIC src/)
target_include_directories(${PROJECT_NAME} PUBLIC external/)
target_include_directories(${PROJECT_NAME} PUBLIC external/SDL2/include)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)