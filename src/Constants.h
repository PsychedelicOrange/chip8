//
// Created by parth on 30/5/25.
//

#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <cmath>
#include <cstdint>

namespace chip8
{
    // the chip8 interpreter used to be stored at start from 0x0 to 0x1ff
    constexpr uint16_t PROGRAM_MEMORY_OFFSET = 0x200;
    constexpr uint16_t FONT_MEMORY_OFFSET = 0x000;
    constexpr uint16_t MEMORY_SIZE = 4096;
    constexpr uint16_t REGISTER_COUNT = 16;
    constexpr uint16_t KEY_COUNT = 16;
    constexpr int CPU_CLOCK_RATE = static_cast<int>(std::pow(10,6));
    constexpr uint8_t TIMER_FREQUENCY = 60;
    constexpr uint8_t FONT_HEIGHT = 5;
    constexpr uint8_t DISPLAY_HEIGHT = 32;
    constexpr uint8_t DISPLAY_WIDTH = 64;
}

#endif //CONSTANTS_H
