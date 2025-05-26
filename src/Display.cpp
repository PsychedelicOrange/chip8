//
// Created by parth on 26/5/25.
//

#include "Display.h"

namespace chip8 {
    void Display::clear()
    {
        for (int i = 0; i < 32; i++)
        {
            for (int j = 0; j < 64; j++)
            {
                buffer[i][j] = 0;
            }
        }
    }

    void Display::checkerBoard()
    {
        bool val = true;
        for (int i = 0; i < 32; i++)
        {
            val = !val;
            for (int j = 0; j < 64; j++)
            {
            val = !val;
                buffer[i][j] = val? 0: 0xFF;
            }
        }
    }

    Display::Display()
    {
        checkerBoard();
    }
} // chip8