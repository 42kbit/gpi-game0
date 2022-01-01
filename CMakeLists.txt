cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

set(PROJECT_NAME gpi-game0)

project(${PROJECT_NAME})

add_executable(${PROJECT_NAME}
src/main.c
src/CMD_Input.c
src/CMD_Window.c
src/CMD_BatchRenderer.c
src/CMD_VertexTypes.c
src/CMD_ChunkData.c
src/CMD_ChunkRenderer.c
src/CMD_Global.c
src/CMD_Meshes.c
src/CMD_World.c
)

add_subdirectory(external/gpi)

target_include_directories(${PROJECT_NAME} 
 PUBLIC external/gpi/src
 PUBLIC external/gpi/external/
 PUBLIC external/gpi/external/SDL2/include
 PUBLIC external/gpi/external/glad/include)

target_link_libraries(${PROJECT_NAME} PUBLIC gpi -lm)

set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)

add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/res $<TARGET_FILE_DIR:${PROJECT_NAME}>/res)
