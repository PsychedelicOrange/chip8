//
// Created by parth on 26/5/25.
//

#include "Emulator.h"

#include <algorithm>
#include <cassert>
#include <iomanip>
#include <iostream>

#include "Util.h"

namespace chip8
{
    void Emulator::run()
    {
        if (wait_for > 0)
        {
            for (int i = 0; i < input.keys.size(); i++)
            {
                if (input.isKeyDown(i))
                {
                    signal_key_down(i);
                    break;
                }
            }
        }
        if (wait_for < 0)
        {
            const Instruction instruction(fetch());
            execute(instruction);
        }
    }

    void Emulator::signal_key_down(uint8_t key)
    {
        sys.registers.at(wait_for) = key;
        wait_for = -1;
    }

    bool Emulator::shouldUpdateDisplay()
    {
        if (display.dirty)
        {
            display.dirty = false;
            return true;
        }
        return false;
    }

    bitmap_t* Emulator::getDisplayDataRef()
    {
        return &display.buffer;
    }

    void Emulator::timer_tick()
    {
        if (sys.delay_timer > 0)
        {
            sys.delay_timer--;
        }
        if (sys.sound_timer > 0)
        {
            sys.sound_timer--;
        }
    }

    std::array<bool, 16>& Emulator::getInputData()
    {
        return input.keys;
    }

    void Emulator::load_program_to_memory(std::vector<uint8_t> program)
    {
        assert(program.size() < chip8::MEMORY_SIZE - PROGRAM_MEMORY_OFFSET);
        std::copy_n(program.begin(), program.size(), sys.memory.begin() + PROGRAM_MEMORY_OFFSET);
    }

    void Emulator::DXYN(uint16_t X, uint16_t Y, uint16_t N)
    {
        const uint8_t x_coord = sys.registers.at(X) % 64;
        const uint8_t y_coord = sys.registers.at(Y) % 32;
        sys.registers.at(0xf) = 0;
        for (int i = 0; i < N; i++)
        {
            if (y_coord + i >= 32) continue;
            const uint8_t row = sys.memory.at(sys.index_register + i);
            for (uint8_t x = 0; x < 8; x++)
            {
                if (x_coord + x >= 64) break;
                if ((row & (0x1 << 7 - x)) != 0) // get x'th bit
                {
                    if (const int value = display.buffer[y_coord + i][x_coord + x]; value != 0xFF)
                    {
                        display.dirty = true;
                        display.buffer[y_coord + i][x_coord + x] = 0xFF;
                    }
                    else
                    {
                        display.dirty = true;
                        display.buffer[y_coord + i][x_coord + x] = 0x00;
                        sys.registers.at(0xf) = 1;
                    }
                }
            }
        }
    }

    void Emulator::execute(const Instruction instruction)
    {
        // incomplete list
        switch (instruction.instruction)
        {
        case 0x00E0:
            display.clear();
            break;
        case 0x00EE:
            sys.program_counter = sys.stack.top();
            sys.stack.pop();
            break;
        default:
            break;
        }

        switch (instruction.I())
        {
        case 0x1:
            sys.program_counter = instruction.NNN();
            break;
        case 0x2:
            sys.stack.push(sys.program_counter);
            sys.program_counter = instruction.NNN();
            break;
        case 0x3:
            if (sys.registers.at(instruction.X()) == instruction.NN())
            {
                sys.program_counter += 2;
            }
            break;
        case 0x4:
            if (sys.registers.at(instruction.X()) != instruction.NN())
            {
                sys.program_counter += 2;
            }
            break;
        case 0x5:
            if (sys.registers.at(instruction.X()) == sys.registers.at(instruction.Y()))
            {
                sys.program_counter += 2;
            }
            break;
        case 0x6:
            sys.registers.at(instruction.X()) = instruction.NN();
            break;
        case 0x7:
            sys.registers.at(instruction.X()) += instruction.NN();
            // don't set the overflow flag
            break;
        case 0x8:
            switch (instruction.N())
            {
            case 0:
                sys.registers.at(instruction.X()) = sys.registers.at(instruction.Y());
                break;
            case 1:
                sys.registers.at(instruction.X()) = sys.registers.at(instruction.X()) | sys.registers.at(
                    instruction.Y());
                break;
            case 2:
                sys.registers.at(instruction.X()) = sys.registers.at(instruction.X()) & sys.registers.at(
                    instruction.Y());
                break;
            case 3:
                sys.registers.at(instruction.X()) = sys.registers.at(instruction.X()) ^ sys.registers.at(
                    instruction.Y());
                break;
            case 4:
                if (sys.registers.at(instruction.X()) > 255 - sys.registers.at(instruction.Y())) sys.registers.at(0xf) =
                    1;
                else sys.registers.at(0xf) = 0;
                sys.registers.at(instruction.X()) = sys.registers.at(instruction.X()) + sys.registers.at(
                    instruction.Y());
                break;
            case 5:
                if (sys.registers.at(instruction.X()) >= sys.registers.at(instruction.Y())) sys.registers.at(0xf) = 1;
                else sys.registers.at(0xf) = 0;
                sys.registers.at(instruction.X()) = sys.registers.at(instruction.X()) - sys.registers.at(
                    instruction.Y());
                break;
            case 6:
                if (shift_op_super_chip_behaviour)
                {
                    sys.registers.at(0xf) = sys.registers.at(instruction.X()) & 1;
                    sys.registers.at(instruction.X()) = (sys.registers.at(instruction.X()) >> 1);
                }
                else
                {
                    sys.registers.at(0xf) = sys.registers.at(instruction.Y()) & 1;
                    sys.registers.at(instruction.X()) = (sys.registers.at(instruction.Y()) >> 1);
                }
                break;
            case 7:
                if (sys.registers.at(instruction.Y()) >= sys.registers.at(instruction.X())) sys.registers.at(0xf) = 1;
                else sys.registers.at(0xf) = 0;
                sys.registers.at(instruction.X()) = sys.registers.at(instruction.Y()) - sys.registers.at(
                    instruction.X());
                break;
            case 0xE:
                if (shift_op_super_chip_behaviour)
                {
                    sys.registers.at(0xf) = (sys.registers.at(instruction.X()) & 0x80) != 0;
                    sys.registers.at(instruction.X()) = (sys.registers.at(instruction.X()) << 1);
                }
                else
                {
                    sys.registers.at(0xf) = (sys.registers.at(instruction.Y()) & 0x80) != 0;
                    sys.registers.at(instruction.X()) = (sys.registers.at(instruction.Y()) << 1);
                }
                break;
            default:
                break;
            }
            break;
        case 0x9:
            if (sys.registers.at(instruction.X()) != sys.registers.at(instruction.Y()))
            {
                sys.program_counter += 2;
            }
            break;
        case 0xA:
            sys.index_register = instruction.NNN();
            break;
        case 0xB:
            if (jump_offset_super_chip_behaviour)
            {
                sys.index_register = instruction.NN() + sys.registers.at(instruction.X());
            }
            else
            {
                sys.index_register = instruction.NNN() + sys.registers.at(0x0);
            }
            break;
        case 0xC:
            sys.registers.at(instruction.X()) = (static_cast<uint8_t>(instruction.NN())) & util::get_random_byte();
            break;
        case 0xD:
            DXYN(instruction.X(), instruction.Y(), instruction.N());
            break;
        case 0xE:
            if (instruction.NN() == 0x9e)
            {
                if (input.isKeyDown(0xF & sys.registers.at(instruction.X())))
                {
                    sys.program_counter += 2;
                }
            }
            else if (instruction.NN() == 0xA1)
            {
                if (!input.isKeyDown(0xF & sys.registers.at(instruction.X())))
                {
                    sys.program_counter += 2;
                }
            }
            break;
        case 0xF:
            switch (instruction.NN())
            {
            case 0x07:
                sys.registers.at(instruction.X()) = sys.delay_timer;
                break;
            case 0x15:
                sys.delay_timer = sys.registers.at(instruction.X());
                break;
            case 0x18:
                sys.sound_timer = sys.registers.at(instruction.X());
                break;
            case 0x1e:
                sys.index_register += sys.registers.at(instruction.X());
                break;
            case 0x0A:
                wait_for = instruction.X();
                break;
            case 0x29:
                sys.index_register = 5 * (sys.registers.at(instruction.X()) & 0x0F);
                break;
            case 0x33:
                {
                    const uint8_t x = sys.registers.at(instruction.X());
                    sys.memory.at(sys.index_register) = x / 100;
                    sys.memory.at(sys.index_register + 1) = (x / 10) % 10;
                    sys.memory.at(sys.index_register + 2) = (x % 10);
                }
                break;
            case 0x55:
                {
                    for (int i = 0; i <= instruction.X(); i++)
                    {
                        sys.memory.at(sys.index_register + i) = sys.registers.at(i);
                    }
                    if (use_temp_index_super_chip_behaviour)
                    {
                        sys.index_register += sys.registers.size();
                    }
                }
                break;
            case 0x65:
                for (int i = 0; i <= instruction.X(); i++)
                {
                    sys.registers.at(i) = sys.memory.at(sys.index_register + i);
                }
                if (!use_temp_index_super_chip_behaviour)
                {
                    sys.index_register += sys.registers.size();
                }
                break;
            default:
                break;
            }
        default:
            break;
        }
    }

    uint16_t Emulator::fetch()
    {
        const uint8_t higerByte = sys.memory.at(sys.program_counter);
        const uint8_t lowerByte = sys.memory.at(sys.program_counter + 1);
        uint16_t instruction = 0x0000;
        instruction |= higerByte << 8;
        instruction |= lowerByte;
        sys.program_counter += 2;
        return instruction;
    }
} // chip8
