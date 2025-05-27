//
// Created by parth on 26/5/25.
//

#ifndef DISPLAY_H
#define DISPLAY_H
#include <cstdint>

namespace chip8
{
    class Display
    {
    public:
        uint8_t buffer[32][64];
        bool dirty;
        void clear();
        void checkerBoard();
        Display();
    };
} // chip8

#endif //DISPLAY_H
