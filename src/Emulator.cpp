//
// Created by parth on 26/5/25.
//

#include "Emulator.h"

#include <algorithm>
#include <cassert>
#include <iomanip>
#include <iostream>

#include "Util.h"

namespace chip8 {
    void Emulator::run()
    {
        const Instruction instruction(fetch());
        execute(instruction);
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

    void Emulator::load_program_to_memory(std::vector<uint8_t> program)
    {
        assert(program.size() < 4096 - PROGRAM_MEMORY_OFFSET);
        std::copy_n(program.begin(), program.size(), sys.memory + PROGRAM_MEMORY_OFFSET);
    }

    void Emulator::DXYN(uint16_t X, uint16_t Y, uint16_t N)
    {
        const uint8_t x_coord = sys.registers[X] % 64;
        const uint8_t y_coord = sys.registers[Y] % 32;
        for (int i = 0; i < N; i++)
        {
            if (y_coord + i >= 32) continue;
            const uint8_t row = sys.memory[sys.index_register + i];
            for (uint8_t x = 0; x < 8; x++)
            {
                if (x_coord + x >= 64) break;
                if ((row & (0x1 << 7 - x)) != 0) // get x'th bit
                {
                    if (const int value = display.buffer[y_coord + i][x_coord + x]; value != 0xFF)
                    //invert and set VF register
                    {
                        display.dirty = true;
                        display.buffer[y_coord + i][x_coord + x] = 0xFF;
                        sys.registers[0xf] = 0;
                    }
                    else
                    {
                        display.dirty = true;
                        display.buffer[y_coord + i][x_coord + x] = 0x00;
                        sys.registers[0xf] = 1;
                    }
                }
            }
        }
    }

    void Emulator::execute(const Instruction instruction)
    {
        std::cout << "Executing instruction: " << std::hex << std::setw(4) << std::setfill('0') << static_cast<int>(
            instruction.instruction) << std::endl;
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
            if (sys.registers[instruction.X()] == instruction.NN())
            {
                sys.program_counter += 2;
            }
            break;
        case 0x4:
            if (sys.registers[instruction.X()] != instruction.NN())
            {
                sys.program_counter += 2;
            }
            break;
        case 0x5:
            if (sys.registers[instruction.X()] == sys.registers[instruction.Y()])
            {
                sys.program_counter += 2;
            }
            break;
        case 0x6:
            sys.registers[instruction.X()] = instruction.NN();
            break;
        case 0x7:
            sys.registers[instruction.X()] += instruction.NN();
            // don't set the overflow flag
            break;
        case 0x8:
            switch (instruction.N())
            {
            case 0:
                sys.registers[instruction.Y()] = sys.registers[instruction.X()];
                break;
            case 1:
                sys.registers[instruction.X()] = sys.registers[instruction.X()] | sys.registers[instruction.Y()];
                break;
            case 2:
                sys.registers[instruction.X()] = sys.registers[instruction.X()] & sys.registers[instruction.Y()];
                break;
            case 3:
                sys.registers[instruction.X()] = sys.registers[instruction.X()] ^ sys.registers[instruction.Y()];
                break;
            case 4:
                if (sys.registers[instruction.X()] > 255 - sys.registers[instruction.Y()]) sys.registers[0xf] = 1;
                else sys.registers[0xf] = 0;
                sys.registers[instruction.X()] = sys.registers[instruction.X()] + sys.registers[instruction.Y()];
                break;
            case 5:
                if (sys.registers[instruction.X()] > sys.registers[instruction.Y()]) sys.registers[0xf] = 1;
                else sys.registers[0xf] = 0;
                sys.registers[instruction.X()] = sys.registers[instruction.X()] - sys.registers[instruction.Y()];
                break;
            case 6:
                if (shift_op_super_chip_behaviour)
                {
                    sys.registers[0xf] = sys.registers[instruction.X()] & 1;
                    sys.registers[instruction.X()] = (sys.registers[instruction.X()] >> 1);
                }
                else
                {
                    sys.registers[0xf] = sys.registers[instruction.Y()] & 1;
                    sys.registers[instruction.X()] = (sys.registers[instruction.Y()] >> 1);
                }
                break;
            case 7:
                if (sys.registers[instruction.Y()] > sys.registers[instruction.X()]) sys.registers[0xf] = 1;
                else sys.registers[0xf] = 0;
                sys.registers[instruction.X()] = sys.registers[instruction.Y()] + sys.registers[instruction.X()];
                break;
            case 0xE:
                if (shift_op_super_chip_behaviour)
                {
                    sys.registers[0xf] = (sys.registers[instruction.X()] & 0x80) != 0;
                    sys.registers[instruction.X()] = (sys.registers[instruction.X()] << 1);
                }
                else
                {
                    sys.registers[0xf] = (sys.registers[instruction.Y()] & 0x80) != 0;
                    sys.registers[instruction.X()] = (sys.registers[instruction.Y()] << 1);
                }
                break;
            default:
                break;
            }
            break;
        case 0x9:
            if (sys.registers[instruction.X()] != sys.registers[instruction.Y()])
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
                sys.index_register = instruction.NN() + sys.registers[instruction.X()];
            }
            else
            {
                sys.index_register = instruction.NNN() + sys.registers[0x0];
            }
            break;
        case 0xC:
            sys.registers[instruction.X()] = (static_cast<uint8_t>(instruction.NN())) & util::get_random_byte();
            break;
        case 0xD:
            DXYN(instruction.X(), instruction.Y(), instruction.N());
            break;
        case 0xE:
            if (instruction.NN() == 0x9e)
            {
                if (input.isKeyDown(instruction.X()))
                {
                    sys.program_counter += 2;
                }
            }
            else if (instruction.NN() == 0xA1)
            {
                if (!input.isKeyDown(instruction.X()))
                {
                    sys.program_counter += 2;
                }
            }
            break;
        case 0xF:
            switch (instruction.NN())
            {
            case 0x07:
                sys.registers[instruction.X()] = sys.delay_timer;
                break;
            case 0x15:
                sys.delay_timer = sys.registers[instruction.X()];
                break;
            case 0x18:
                sys.sound_timer = sys.registers[instruction.X()];
                break;
            case 0x1e:
                sys.index_register += sys.registers[instruction.X()];
                break;
            case 0x0A:
                // TODO: wait for key press [ don't stop timers ]
                break;
            case 0x29:
                // TODO: font rendering
                sys.index_register = 5 * (sys.registers[instruction.X()] & 0x0F);
                break;
            case 0x33:
                {
                    const auto x = static_cast<uint8_t>(instruction.X());
                    sys.memory[sys.index_register] = x / 100;
                    sys.memory[sys.index_register + 1] = (x % 100) / 10;
                    sys.memory[sys.index_register + 1] = x % 10;
                }
                break;
            case 0x55:
                std::copy_n(sys.registers.begin(), sys.registers.size(), sys.memory + sys.index_register);
                if (!use_temp_index_super_chip_behaviour)
                {
                    sys.index_register += sys.registers.size();
                }
                break;
            case 0x65:
                std::copy_n(sys.memory + sys.index_register, sys.registers.size(), sys.registers.begin());
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
        const uint8_t higerByte = sys.memory[sys.program_counter];
        const uint8_t lowerByte = sys.memory[sys.program_counter + 1];
        uint16_t instruction = 0x0000;
        instruction |= higerByte << 8;
        instruction |= lowerByte;
        sys.program_counter += 2;
        return instruction;
    }
} // chip8