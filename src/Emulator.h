//
// Created by parth on 26/5/25.
//

#ifndef EMULATOR_H
#define EMULATOR_H

#include <cmath>
#include <optional>

#include "Display.h"
#include "Instruction.h"
#include "System.h"

namespace chip8
{
    class Emulator
    {
        System sys{};
        std::array<bool,KEY_COUNT> key_pressed{0};
        Display display{};
        std::optional<uint8_t> wait_for;
        bool shift_op_super_chip_behaviour = true;
        bool jump_offset_super_chip_behaviour = false;
        bool use_temp_index_super_chip_behaviour = false;
    public :
        int instructions_per_frame = 10;
        Emulator() = default;
        Emulator(bool shift_op_super_chip_behaviour, bool jump_offset_super_chip_behaviour,
                 bool use_temp_index_super_chip_behaviour,int instructions_per_frame) :
            shift_op_super_chip_behaviour(shift_op_super_chip_behaviour),
            jump_offset_super_chip_behaviour(jump_offset_super_chip_behaviour),
            use_temp_index_super_chip_behaviour(use_temp_index_super_chip_behaviour),
            instructions_per_frame(instructions_per_frame)
        {
        };

        // memory
        void load_program_to_memory(std::vector<uint8_t> program);

        // input
        std::array<bool, KEY_COUNT>& getInputData();

        // display
        [[nodiscard]] bool shouldUpdateDisplay();
        bitmap_t* getDisplayDataRef();

        // CPU
        void timer_tick();
        void run();

    private:
        void DXYN(uint16_t X, uint16_t Y, uint16_t N);;
        void execute(Instruction instruction);
        [[nodiscard]] uint16_t fetch();
        void signal_key_down(uint8_t key);
    };
} // chip8

#endif //EMULATOR_H
