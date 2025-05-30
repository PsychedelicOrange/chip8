//
// Created by parth on 26/5/25.
//

#ifndef RENDERER_H
#define RENDERER_H
#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include "Types.h"

class Renderer {
    const bitmap_t* screen_data;
    unsigned int shader;
    unsigned int texture;
    unsigned int vao;
public:
    GLFWwindow* window;
    int getRefreshRate() const;

    explicit Renderer(const bitmap_t* screen_data);
    void updateTexture();
    void render() const;
};

#endif //RENDERER_H
