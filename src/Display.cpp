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
            for (auto & j : i)
            {
                j = 0;
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
            for (auto& j : i)
            {
                val = !val;
                j = val? 0: 0xFF;
            }
        }
    }

    Display::Display()
    {
        dirty = true;
        clear();
    }
} // chip8