cmake_minimum_required(VERSION 3.26)
project(cudose C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -lSDL2 -lSDL2_ttf -lSDL2_gfx")

add_executable(cudose main.c
        shuntingYard.h
        lexer.h
        tree.h
        evaluate.h
        gui.h)

target_link_libraries(cudose m)