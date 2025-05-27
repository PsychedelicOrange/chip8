//
// Created by parth on 26/5/25.
//

#include "Display.h"

namespace chip8 {
    void Display::clear()
    {
        dirty = true;
        for (auto & i : buffer)
        {
            for (int j = 0; j < 64; j++)
            {
                i[j] = 0;
            }
        }
    }

    void Display::checkerBoard()
    {
        dirty = true;
        bool val = true;
        for (auto & i : buffer)
        {
            val = !val;
            for (int j = 0; j < 64; j++)
            {
            val = !val;
                i[j] = val? 0: 0xFF;
            }
        }
    }

    Display::Display()
    {
        clear();
    }
} // chip8