//
// Created by parth on 26/5/25.
//

#ifndef INPUT_H
#define INPUT_H
#include <cstdint>
#include <iostream>

namespace chip8 {

class Input {
public:
    bool keys[16] = {};
    bool isKeyDown(uint8_t key)
    {
        return keys[static_cast<int>(key)] ;
    }
    Input()
    {
        for (auto& key : keys)
        {
            key = false;
        }
    }
};

} // chip8

#endif //INPUT_H
