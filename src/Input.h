//
// Created by parth on 26/5/25.
//

#ifndef INPUT_H
#define INPUT_H

namespace chip8 {
    enum Key
    {
        KEY_0, KEY_1, KEY_2, KEY_3, KEY_4, KEY_5,
        KEY_6, KEY_7, KEY_8, KEY_9, KEY_A,
        KEY_B, KEY_C, KEY_D, KEY_E, KEY_F
    };

class Input {
    bool keys[16] = {};
    bool isKeyPressed(Key key)
    {
        return 0;
    }
};

} // chip8

#endif //INPUT_H
