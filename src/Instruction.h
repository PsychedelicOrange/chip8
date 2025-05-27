//
// Created by parth on 28/5/25.
//

#ifndef INSTRUCTION_H
#define INSTRUCTION_H
#include <cstdint>


struct Instruction
{
    uint16_t instruction;

    explicit Instruction(uint16_t instruction): instruction(instruction)
    {
    }

    uint16_t I() const
    {
        return (instruction & 0xF000) >> 12;
    }

    uint16_t X() const
    {
        return (instruction & 0x0F00) >> 8;
    }

    uint16_t Y() const
    {
        return (instruction & 0x00F0) >> 4;
    }
    uint16_t N() const
    {
        return (instruction & 0x000F);
    }

    uint16_t NN() const
    {
        return (instruction & 0x00FF);
    }

    uint16_t NNN() const
    {
        return (instruction & 0x0FFF);
    }
};




#endif //INSTRUCTION_H
