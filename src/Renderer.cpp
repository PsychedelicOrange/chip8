//
// Created by parth on 26/5/25.
//

#include "Renderer.h"

#include <iostream>
#include <fstream>
#include <set>

#include "GLFW/glfw3.h"
#include "Util.h"

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
void *create_glfw_window(const int SCR_WIDTH, const int SCR_HEIGHT, const char *name, void *monitor)
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

void init_glad()
{
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "failed to initialize glad";
    }
    const GLubyte *vendor = glGetString(GL_VENDOR);     // Returns the vendor
    const GLubyte *renderer = glGetString(GL_RENDERER); // Returns a hint to the model
    std::cout <<"GPU information" << std::endl;
    std::cout <<"Vendor:"<< vendor << std::endl;
    std::cout <<"Renderer:"<< renderer << std::endl;
}

unsigned int setup_screen_quad(void)
{
    // Vertex data for the rectangle (two triangles forming a quad) ( flipping the UV because of buffer is Y flipped)
    // clang-format off
    float vertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
        // positions   // texCoords
        -1.0f,  1.0f,  0.0f, 0.0f,
        -1.0f, -1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 1.0f,

        -1.0f,  1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 1.0f,
         1.0f,  1.0f,  1.0f, 0.0f
    };

    // clang-format on
    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(2 * sizeof(float)));
    glBindVertexArray(0);
    return VAO;
}
unsigned int create_program(unsigned int vertexShader, unsigned int fragmentShader)
{
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    int success = 0;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, 0, infoLog);
        std::cerr << "failed to link shaders: "<< infoLog << std::endl;
        return -1;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return shaderProgram;
}
unsigned int compile_shader(const char *shaderCode, int shaderType)
{
    unsigned int shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderCode, 0);
    glCompileShader(shader);
    // check for shader compile errors
    int success = 0;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, 512, 0, infoLog);
        std::cerr << "failed to link shaders: "<< infoLog << std::endl;
        return -1;
    }
    return shader;
}

unsigned int create_shader(char *vertexPath, char *fragPath)
{
    std::string vertexShaderCode = util::read_string_from_file(vertexPath);
    std::string fragmentShaderCode = util::read_string_from_file(fragPath);
    unsigned int vertexShader = compile_shader(vertexShaderCode.c_str(), GL_VERTEX_SHADER);
    unsigned int fragmentShader = compile_shader(fragmentShaderCode.c_str(), GL_FRAGMENT_SHADER);
    return create_program(vertexShader, fragmentShader);
}

unsigned int create_texture(const uint8_t* data,unsigned int shader)
{
    unsigned int tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);

    { // set sampling properties
            // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }

    //glPixelStorei(GL_UNPACK_ALIGNMENT, 8);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, 64, 32, 0, GL_RED, GL_UNSIGNED_BYTE, data);

    return tex;
}
int Renderer::getRefreshRate() const
{
    return glfwGetVideoMode(glfwGetPrimaryMonitor())->refreshRate;
}
int Renderer::shouldWindowClose() const
{
    return glfwWindowShouldClose(window);
}

Renderer::Renderer(const uint8_t* screen_data)
{
    this->screen_data = screen_data;
    init_glfw(4,1); // last supported on MacOS
    window = static_cast<GLFWwindow*>(create_glfw_window(800,400,"chip8emu", nullptr));
    init_glad();
    glfwSwapInterval(1); // enable vsync for our emulation speed to work properly

    vao = setup_screen_quad();
    shader = create_shader("/home/parth/CLionProjects/chip8/src/screen.vert","/home/parth/CLionProjects/chip8/src/screen.frag");
    glUseProgram(shader);
    glUniform1i(glGetAttribLocation(shader,"screenTexture"), 0);
    texture = create_texture(screen_data,shader);
}

void Renderer::updateTexture()
{
    glDeleteTextures(1,&texture);
    texture = create_texture(screen_data,shader);
}
void Renderer::render() const
{
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.5,0.5,0.5,1);

        glUseProgram(shader);
        glBindVertexArray(vao);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glfwSwapBuffers(window);
        glfwPollEvents();
}

