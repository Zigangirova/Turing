cmake_minimum_required(VERSION 3.15)
project(Turing C)

set(CMAKE_C_STANDARD 99)

include_directories(.)

add_executable(Turing
        input.c
        output.c
        process.c
        start.c
        turing.h
        )