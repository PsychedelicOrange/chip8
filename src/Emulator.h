//
// Created by parth on 26/5/25.
//

#ifndef EMULATOR_H
#define EMULATOR_H

#include <cmath>

#include "Display.h"
#include "Input.h"
#include "Instruction.h"
#include "System.h"

namespace chip8
{
    class Emulator
    {
        System sys{};
        Input input{};
        bool shift_op_super_chip_behaviour = true;
        bool jump_offset_super_chip_behaviour = false;
        bool use_temp_index_super_chip_behaviour = true;

    public :
        Display display{};
        Emulator() = default;

        Emulator(bool shift_op_super_chip_behaviour, bool jump_offset_super_chip_behaviour) :
            shift_op_super_chip_behaviour(shift_op_super_chip_behaviour),
            jump_offset_super_chip_behaviour(jump_offset_super_chip_behaviour)
        {
        };

        void run();
        void timer_tick();

        void load_program_to_memory(std::vector<uint8_t> program);

    private:
        void DXYN(uint16_t X, uint16_t Y, uint16_t N);;

        void execute(Instruction instruction);

        [[nodiscard]] uint16_t fetch();
    };
} // chip8

#endif //EMULATOR_H
