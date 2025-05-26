cmake_minimum_required(VERSION 3.31)
project(chip8)

set(CMAKE_CXX_STANDARD 17)

add_executable(chip8 src/main.cpp
        src/System.cpp
        src/System.h
        src/Emulator.cpp
        src/Emulator.h
        src/Display.cpp
        src/Display.h
        src/Input.cpp
        src/Input.h
        src/Renderer.cpp
        src/Renderer.h
        src/glad.c
        src/Util.cpp
        src/Util.h)

target_link_libraries( chip8
        PRIVATE
        glfw3
        X11 # for glfw3
        m # math
)
