//
// Created by parth on 26/5/25.
//

#ifndef RENDERER_H
#define RENDERER_H
#include "glad/glad.h"
#include <GLFW/glfw3.h>

class Renderer {
    const uint8_t* screen_data;
    unsigned int shader;
    unsigned int texture;
    unsigned int vao;
    GLFWwindow* window;
public:
    int getRefreshRate() const;
    [[nodiscard]] int shouldWindowClose() const;

    explicit Renderer(const uint8_t* screen_data);
    void updateTexture();
    void render() const;
};

#endif //RENDERER_H
