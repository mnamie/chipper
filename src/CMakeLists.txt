find_package(SDL2 REQUIRED CONFIG REQUIRED COMPONENTS SDL2)

add_executable(
    ${PROJECT_NAME}
    main.cpp
    chip8.cpp
    instruction_set.cpp
    io.cpp
)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    target_compile_definitions(${PROJECT_NAME} PUBLIC "IS_LINUX")
endif()
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    target_compile_definitions(${PROJECT_NAME} PUBLIC "IS_WINDOWS")
endif()

target_link_libraries(${PROJECT_NAME} SDL2::SDL2main)
target_link_libraries(${PROJECT_NAME} SDL2::SDL2)