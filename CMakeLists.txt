cmake_minimum_required(VERSION 3.22.1)

project(chipper)

find_package(SDL2 REQUIRED CONFIG REQUIRED COMPONENTS SDL2)

add_executable(
    ${PROJECT_NAME}
    src/main.cpp
    src/chip8.cpp
    src/instruction_set.cpp
    src/io.cpp
)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    target_compile_definitions(${PROJECT_NAME} PUBLIC "IS_LINUX")
endif()
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    target_compile_definitions(${PROJECT_NAME} PUBLIC "IS_WINDOWS")
endif()

target_link_libraries(${PROJECT_NAME} SDL2::SDL2main)
target_link_libraries(${PROJECT_NAME} SDL2::SDL2)