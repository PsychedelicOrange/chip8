//
// Created by parth on 30/5/25.
//

#include "Window.h"

void init_glfw(const int gl_major, const int gl_minor)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, gl_major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, gl_minor);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
}

static GLFWwindow* create_glfw_window(const int SCR_WIDTH, const int SCR_HEIGHT, const char* name,
                                       void* monitor)
{
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, name, static_cast<GLFWmonitor*>(monitor), nullptr);
    if (window == nullptr)
    {
        std::cerr << "Unable to create glfw window";
        glfwTerminate();
    }
    glfwMakeContextCurrent(window);
    return window;
}

Window::Window(const int resolution_x, const int resolution_y)
{
    init_glfw(4,1); // last supported on MacOS
    this->window = static_cast<GLFWwindow*>(create_glfw_window(resolution_x,resolution_y,"chip8emu", nullptr));
}

void Window::updateInputKeys(std::array<bool, 16>& keys) const
{
    for (const auto [glfwKey, key] : global_input::keys)
    {
        keys[key] = glfwGetKey(window,glfwKey) == GLFW_PRESS;
    }
}

bool Window::shouldWindowClose() const
{
    return glfwWindowShouldClose(window);
}

void Window::swapBuffersAndPollEvents() const
{
    glfwSwapBuffers(window);
    glfwPollEvents();
}
