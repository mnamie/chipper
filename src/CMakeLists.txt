add_executable(
    ${PROJECT_NAME}
    main.c
    chip8.c 
    instruction_set.c 
    io.c     
)

target_link_libraries(${PROJECT_NAME} SDL2)