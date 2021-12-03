cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

set(PROJECT_NAME gpi-game0)

project(${PROJECT_NAME})

add_executable(${PROJECT_NAME} 
src/main.cpp
src/CMD_Input.cpp
src/CMD_Window.cpp
)

add_subdirectory(external/gpi)

target_include_directories(${PROJECT_NAME} 
 PUBLIC external/gpi/src
 PUBLIC external/gpi/external/
 PUBLIC external/gpi/external/SDL2/include
 PUBLIC external/gpi/external/glad/include)

target_link_libraries(${PROJECT_NAME} PUBLIC gpi)

set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)

add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/res $<TARGET_FILE_DIR:${PROJECT_NAME}>/res)