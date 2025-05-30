cmake_minimum_required(VERSION 3.1)
project(chip8)

set(CMAKE_CXX_STANDARD 17)


add_subdirectory(vendor/glfw)


# Path to glad directory
set(GLAD_SOURCES_DIR "${PROJECT_SOURCE_DIR}/vendor/glad/")
# Path to glad cmake files
add_subdirectory("${GLAD_SOURCES_DIR}/cmake" glad_cmake)

# glad settings
glad_add_library(glad_gl_core_41 STATIC API gl:core=4.6 LOCATION ${GLAD_SOURCES_DIR}/build)

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
        src/Util.cpp
        src/Util.h
        src/Instruction.h
        src/Window.cpp
        src/Window.h
        src/Types.h
        vendor/glad/build/src/gl.c)


# Platform-specific configuration
if(WIN32)
    set_target_properties(chip8 PROPERTIES
            VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )

elseif(APPLE)
    target_link_libraries(chip8 PRIVATE
            "-framework Cocoa"
            "-framework IOKit"
            "-framework CoreVideo"
            "-framework OpenGL"
    )
elseif(UNIX)
    target_link_libraries(chip8 PRIVATE X11 m)
endif()


    target_link_libraries( chip8
        PRIVATE
        glad_gl_core_41
        glfw
)
