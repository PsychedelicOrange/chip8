//
// Created by parth on 26/5/25.
//

#include "System.h"

namespace chip8
{

    void System::load_font()
    {
        uint16_t offset = FONT_MEMORY_OFFSET;
        for (const auto character : font)
        {
            for (int j = 0; j < 5; j++)
            {
                memory[offset++] = character[j];
            }
        }
    }

    System::System()
    {
        load_font();
    }
} // chip8
