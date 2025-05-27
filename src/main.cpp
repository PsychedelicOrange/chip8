#include <cmath>
#include <iostream>

#include "Emulator.h"
#include "Renderer.h"
#include "System.h"
#include "Util.h"

int main(int argc, char** argv)
{
    std::vector<uint8_t> program = util::read_byte_array_from_file("/home/parth/CLionProjects/chip8/maze.ch8");
    chip8::Emulator emulator;
    emulator.load_program_to_memory(program);
    Renderer renderer(&emulator.display.buffer[0][0]);
    const int refresh_rate = renderer.getRefreshRate();
    const int cycles_per_frame =  chip8::CHIP8_CPU_CLOCK_RATE / refresh_rate;
    const int timer_frequency = 60;
    const int timer_ticks_per_frame = timer_frequency/ refresh_rate;
    while (!renderer.shouldWindowClose()){
        for (int cycle = 0; cycle < timer_ticks_per_frame; cycle++)
        {
            emulator.timer_tick();
            emulator.run();
        }
        for (int cycle = 0; cycle < cycles_per_frame - timer_ticks_per_frame; cycle++)
        {
            emulator.run();
        }
        if (emulator.display.dirty)
        {
            renderer.updateTexture();
        }
        renderer.render();
    }
    glfwTerminate();
    return 0;
}
