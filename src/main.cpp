#include <thread>

#include "Emulator.h"
#include "Renderer.h"
#include "Util.h"
#include "Window.h"

inline void loop_frequency(double frequency_hz)
{
    static double sleep_begin = glfwGetTime();
    static double work_begin = glfwGetTime();
    sleep_begin = glfwGetTime();
    double work_time = (sleep_begin - work_begin) * 1000;
    if(work_time < 1000.0/frequency_hz)
    {
        std::this_thread::sleep_for(std::chrono::duration<double,std::milli>(1000.0/frequency_hz - work_time));
    }
    work_begin = glfwGetTime();
}

int main(int argc, char** argv)
{
    const Window window(800,400);
    if (argc == 1)
    {
        std::cerr << "Please specify rom path!";
        return 1;
    }
    chip8::Emulator emulator;
    emulator.load_program_to_memory(util::read_byte_array_from_file(argv[1]));
    Renderer renderer(emulator.getDisplayDataRef());

    while (!window.shouldWindowClose())
    {
        loop_frequency(60);
        window.updateInputKeys(emulator.getInputData());
        emulator.timer_tick();
        for (int i = 0; i < emulator.instructions_per_frame; i++)
        {
            emulator.run();
        }
        if (emulator.shouldUpdateDisplay())
        {
            renderer.updateTexture();
            renderer.render();
        }
        window.swapBuffersAndPollEvents();
    }
    return 0;
}
