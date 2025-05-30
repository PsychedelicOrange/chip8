//
// Created by parth on 26/5/25.
//

#ifndef DISPLAY_H
#define DISPLAY_H
#include <array>
#include <cstdint>
#include "Types.h"

namespace chip8
{
    struct Display
    {
        bitmap_t buffer{};
        bool dirty;
        void clear();
        void checkerBoard();
        Display();
    };
} // chip8

#endif //DISPLAY_H
