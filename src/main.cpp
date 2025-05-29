#include <cmath>
#include <iostream>
#include <array>
#include <unordered_map>

#include "Emulator.h"
#include "Renderer.h"
#include "System.h"
#include "Util.h"


void fillInput(GLFWwindow* window, chip8::Input& input)
{
    static constexpr std::array<std::pair<int, int>,16> keys = {{
        {GLFW_KEY_0, 0}, {GLFW_KEY_1, 1}, {GLFW_KEY_2, 2}, {GLFW_KEY_3, 3},
        {GLFW_KEY_4, 4}, {GLFW_KEY_5, 5}, {GLFW_KEY_6, 6}, {GLFW_KEY_7, 7},
        {GLFW_KEY_8, 8}, {GLFW_KEY_9, 9}, {GLFW_KEY_A, 0xA}, {GLFW_KEY_B, 0xB},
        {GLFW_KEY_C, 0xC}, {GLFW_KEY_D, 0xD}, {GLFW_KEY_E, 0xE}, {GLFW_KEY_F, 0xF}
    }};
    for (const auto [glfwKey, key] : keys)
    {
        input.keys[key] = glfwGetKey(window,glfwKey) == GLFW_PRESS;
    }
}

int main(int argc, char** argv)
{
    std::vector<uint8_t> program = util::read_byte_array_from_file("/home/parth/CLionProjects/chip8/ibm.ch8");
    chip8::Emulator emulator;
    emulator.load_program_to_memory(program);
    Renderer renderer(&emulator.display.buffer[0][0]);
    const int refresh_rate = renderer.getRefreshRate();
    const int cycles_per_frame =  chip8::CHIP8_CPU_CLOCK_RATE / refresh_rate;
    constexpr int timer_frequency = 60;
    const int timer_ticks_per_frame = timer_frequency/ refresh_rate;
    while (!renderer.shouldWindowClose()){
        emulator.timer_tick();
        emulator.run();
        if (emulator.display.dirty)
        {
            renderer.updateTexture();
        }
        renderer.render();
        fillInput(renderer.window, emulator.input);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}
