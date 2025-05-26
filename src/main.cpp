#include <iostream>

#include "Emulator.h"
#include "Renderer.h"
#include "System.h"
#include "Util.h"


int main(int argc, char** argv)
{
    std::vector<uint8_t> program = util::read_byte_array_from_file("/home/parth/CLionProjects/chip8/ibm.ch8");
    chip8::Emulator emulator;
    emulator.load_program_to_memory(program);
    for (int i = 0; i < program.size()/2; i ++)
    {
        emulator.run();
    }
    Renderer renderer(&emulator.display.buffer[0][0]);
    renderer.render();
    return 0;
}
