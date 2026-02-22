#include <cmath>
#include <fmt/core.h>
#include <iostream>
#include <fmt/format.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <ostream>

#include "shaders/shader.hpp"
#include "shaders/program.hpp"
#include "buffers/buffer_object.hpp"
#include "buffers/vertex_array.hpp"
#include "textures/texture.hpp"

void framebufferSizeCallback(GLFWwindow*, int width, int height) {
    glViewport(0, 0, width, height);
}

void processEvents(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}

int main() {
    if (glfwInit() == GLFW_FALSE) {
        std::cerr << "Failed to initialize GLFW";
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_FALSE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Hello World!", nullptr, nullptr);
    if (window == nullptr) {
        glfwTerminate();
        std::cerr << "Failed to create window";
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        std::cerr << "Failed to initialize glad";
        glfwTerminate();
        return -1;
    }

    GLint max_va_num;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &max_va_num);
    fmt::print("Max number of vertex atributes is {}\n", max_va_num);

    GLint major_ver, minor_ver;
    glGetIntegerv(GL_MAJOR_VERSION, &major_ver);
    glGetIntegerv(GL_MINOR_VERSION, &minor_ver);
    fmt::print("Installed OpenGL vertion in {}.{}\n", major_ver, minor_ver);
    std::cout << std::flush;

    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    // clang-format off
    float verticies[] = {
        // Vertext coords // Textrue coords
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
         0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
         0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
        -0.5f,  0.5f, 0.0f, 0.0f, 1.0f
    };

    GLuint elements[] = {
        0, 1, 2,
        2, 3, 0
    };
    // clang-format on

    TVertextArray VAO;
    VAO.Bind();

    TBufferObject<EBufferVariant::Vertex> VBO(std::span{verticies});
    VBO.Bind();
    TBufferObject<EBufferVariant::Element> EBO(std::span{elements});
    EBO.Bind();

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    TShader<EShaderVariant::Vertex> vertex_shader("assets/shaders/vertex_shader.glsl");
    TShader<EShaderVariant::Fragment> fragment_shader("assets/shaders/fragment_shader.glsl");
    TShaderProgram shader_program(vertex_shader, fragment_shader);

    shader_program.Use();
    TTexture texture_1("assets/textures/container.jpg", GL_RGB, GL_TEXTURE0);
    shader_program.SetUnifiorm("tex1", 0);
    TTexture texture_2("assets/textures/awesomeface.png", GL_RGBA, GL_TEXTURE1);
    shader_program.SetUnifiorm("tex2", 1);

    while (!glfwWindowShouldClose(window)) {
        processEvents(window);

        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader_program.Use();
        texture_1.Bind();
        texture_2.Bind();
        VAO.Bind();

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
