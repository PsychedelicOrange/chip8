//
// Created by parth on 26/5/25.
//

#ifndef INPUT_H
#define INPUT_H
#include <array>
#include <cstdint>
#include <iostream>

namespace chip8 {

class Input {
public:
    std::array<bool,16> keys;
    [[nodiscard]] bool isKeyDown(const uint8_t key) const
    {
        return keys[key] ;
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
