//
// Created by parth on 26/5/25.
//

#ifndef SYSTEM_H
#define SYSTEM_H
#include <cmath>
#include <cstdint>
#include <stack>
#include <vector>

namespace chip8 {

    // the chip8 interpreter used to be stored at start from 0x0 to 0x1ff
    constexpr uint16_t PROGRAM_MEMORY_OFFSET = 0x200;
    constexpr uint16_t FONT_MEMORY_OFFSET = 0x000;
    constexpr uint16_t MEMORY_SIZE = 4096;
    constexpr uint16_t REGISTER_COUNT = 16;
    constexpr int CHIP8_CPU_CLOCK_RATE = static_cast<int>(std::pow(10,6));

    class System {
    public:
        uint8_t memory[MEMORY_SIZE]{};
        uint16_t program_counter = PROGRAM_MEMORY_OFFSET;
        uint16_t index_register{};
        std::stack<uint16_t> stack;
        std::vector<uint8_t> registers{REGISTER_COUNT};
        uint8_t delay_timer{};
        uint8_t sound_timer{};
        System();
    private:
        void load_font();
    };
} // chip8

#endif //SYSTEM_H