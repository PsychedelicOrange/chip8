//
// Created by parth on 30/5/25.
//

#ifndef WINDOW_H
#define WINDOW_H
#include <array>
#include <iostream>
#include <optional>
#include <unordered_map>

#include "GLFW/glfw3.h"

namespace global_input
{
    static const std::unordered_map<int, uint8_t> keys = {{
        {GLFW_KEY_1, 1}, {GLFW_KEY_2, 2}, {GLFW_KEY_3, 3},{GLFW_KEY_4, 0xC},
        {GLFW_KEY_Q, 4}, {GLFW_KEY_W, 5}, {GLFW_KEY_E, 6},{GLFW_KEY_R, 0xD},
        {GLFW_KEY_A, 7}, {GLFW_KEY_S, 8}, {GLFW_KEY_D, 9},{GLFW_KEY_F, 0xE},
        {GLFW_KEY_Z,0xA},{GLFW_KEY_X, 0},{GLFW_KEY_C, 0xB},{GLFW_KEY_V, 0xF}
    }};

}

class Window {

private:
    GLFWwindow* window;
public:
    Window(int resolution_x, int resolution_y);
    // disallow copying
    Window& operator=(Window & window) = delete;
    Window (Window & window) = delete;
    ~Window()
    {
        window = nullptr;
        glfwTerminate();
    }
    void updateInputKeys(std::array<bool, 16>& keys) const;
    bool shouldWindowClose() const;
    void swapBuffersAndPollEvents() const;
};



#endif //WINDOW_H
