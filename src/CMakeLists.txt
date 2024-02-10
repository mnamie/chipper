add_executable(
    ${PROJECT_NAME}
    main.cpp
    chip8.cpp
    instruction_set.cpp
    io.cpp
)

target_link_libraries(${PROJECT_NAME} SDL2)
