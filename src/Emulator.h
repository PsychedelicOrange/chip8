//
// Created by parth on 26/5/25.
//

#ifndef EMULATOR_H
#define EMULATOR_H
#include <algorithm>
#include <cassert>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <set>
#include <vector>

#include "Display.h"
#include "Input.h"
#include "System.h"

namespace chip8
{
    class Instruction
    {

    public:
        uint16_t instruction;
        explicit Instruction(uint16_t instruction): instruction(instruction){}

        [[nodiscard]] uint16_t I() const
        {
            return (instruction & 0xF000) >> 12;
        }

        [[nodiscard]] uint16_t X() const
        {
            return (instruction & 0x0F00) >> 8;
        }

        [[nodiscard]] uint16_t Y() const
        {
            return (instruction & 0x00F0) >> 4;
        }

        [[nodiscard]] uint16_t N() const
        {
            return (instruction & 0x000F);
        }

        [[nodiscard]] uint16_t NN() const
        {
            return (instruction & 0x00FF);
        }

        [[nodiscard]] uint16_t NNN() const
        {
            return (instruction & 0x0FFF);
        }
    };

    class Emulator
    {
        System sys{};
        Input input{};

    public :
        Display display{};

        void run()
        {
            const Instruction instruction(fetch());
            execute(instruction);
        }

        void load_program_to_memory(std::vector<uint8_t> program)
        {
            assert(program.size() < 4096 - PROGRAM_MEMORY_OFFSET);
            std::copy_n(program.begin(),program.size(),sys.memory + PROGRAM_MEMORY_OFFSET);
        }

    private:
        void DXYN(uint16_t X,uint16_t Y, uint16_t N)
        {
            const uint8_t x_coord = sys.registers[X] % 64;
            const uint8_t y_coord = sys.registers[Y] % 32;
            for (int i = 0; i < N; i++)
            {
                const uint8_t row = sys.memory[sys.index_register+i];
                if (y_coord + i >= 32) continue;
                for (uint8_t x = 0; x < 8; x++)
                {
                    if (x_coord + x >= 64) break;
                    if ((row & (0x1 << 7-x)) != 0) // get x'th bit
                    {
                        if(const int value = display.buffer[y_coord+i][x_coord + x]; value != 0xFF) //invert and set VF register
                        {
                            display.buffer[y_coord+i][x_coord + x] = 0xFF;
                            sys.registers[0xf] = 0;
                        }else{
                            display.buffer[y_coord+i][x_coord + x] = 0x00;
                            sys.registers[0xf] = 1;
                        }
                    }
                }
            }
        };

        void execute(const Instruction instruction)
        {
            std::cout << "Executing instruction: " << std::hex << std::setw(4) << std::setfill('0')<< static_cast<int>(instruction.instruction) << std::endl;
            // incomplete list
            switch (instruction.instruction)
            {
            case 0x00E0:
                display.clear();
                break;
            default:
                break;
            }

            switch (instruction.I())
            {
            case 0x1:
                sys.program_counter = instruction.NNN();
                break;
            case 0x6:
                sys.registers[instruction.X()] = instruction.NN();
                break;
            case 0x7:
                sys.registers[instruction.X()] += instruction.NN();
                // don't set the overflow flag
                break;
            case 0xA:
                sys.index_register = instruction.NNN();
                break;
            case 0xD:
                DXYN(instruction.X(), instruction.Y(), instruction.N());
                break;
            default:
                break;
            }
        }

        [[nodiscard]] uint16_t fetch()
        {
            const uint8_t higerByte = sys.memory[PROGRAM_MEMORY_OFFSET + sys.program_counter];
            const uint8_t lowerByte = sys.memory[PROGRAM_MEMORY_OFFSET + sys.program_counter + 1];
            uint16_t instruction = 0x0000;
            instruction |= higerByte << 8;
            instruction |= lowerByte;
            sys.program_counter += 2;
            return instruction;
        }
    };
} // chip8

#endif //EMULATOR_H
