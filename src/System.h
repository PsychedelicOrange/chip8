//
// Created by parth on 26/5/25.
//

#ifndef SYSTEM_H
#define SYSTEM_H
#include <array>
#include <cmath>
#include <cstdint>
#include <stack>
#include <vector>

namespace chip8 {

    // clang-format off
    static constexpr uint8_t font[16][5] = {
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
        };
    // clang-format on

    // the chip8 interpreter used to be stored at start from 0x0 to 0x1ff
    constexpr uint16_t PROGRAM_MEMORY_OFFSET = 0x200;
    constexpr uint16_t FONT_MEMORY_OFFSET = 0x000;
    constexpr uint16_t MEMORY_SIZE = 4096;
    constexpr uint16_t REGISTER_COUNT = 16;
    constexpr int CHIP8_CPU_CLOCK_RATE = static_cast<int>(std::pow(10,6));

    class System {
    public:
        std::array<uint8_t, MEMORY_SIZE> memory{0};
        uint16_t program_counter = PROGRAM_MEMORY_OFFSET;
        uint16_t index_register = 0;
        std::stack<uint16_t> stack;
        std::array<uint8_t, REGISTER_COUNT> registers{0};
        uint8_t delay_timer{};
        uint8_t sound_timer{};
        System();
    private:
        void load_font();
    };
} // chip8

#endif //SYSTEM_H