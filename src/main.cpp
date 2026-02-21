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
    float verticies_1[] = {
        -0.9f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,// left 
        -0.0f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,// right
        -0.45f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f // top 
    };

    float verticies_2[] = {
         0.3f, -0.5f, 0.0f,  // left
         0.9f, -0.5f, 0.0f,  // right
         0.45f, 0.5f, 0.0f   // top 
    };
    // clang-format on

    TVertextArray VAO_1;
    TBufferObject<EBufferVariant::Vertex> VBO_1(std::span{verticies_1});
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), NULL);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    TVertextArray VAO_2;
    TBufferObject<EBufferVariant::Vertex> VBO_2(std::span{verticies_2});
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL);
    glEnableVertexAttribArray(0);

    TShader<EShaderVariant::Vertex> vertex_shader("../assets/shaders/vertex_shader.glsl");
    TShader<EShaderVariant::Fragment> fragment_shader_1("../assets/shaders/fragment_shader.glsl");
    TShader<EShaderVariant::Fragment> fragment_shader_2("../assets/shaders/fragment_shader_new.glsl");

    TShaderProgram shader_program_1(vertex_shader, fragment_shader_1);
    TShaderProgram shader_program_2(vertex_shader, fragment_shader_2);

    while (!glfwWindowShouldClose(window)) {
        processEvents(window);

        glClearColor(0.1f, 0.7f, 0.4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        float pulse = static_cast<float>(std::abs(std::sin(glfwGetTime())));
        float offset = static_cast<float>(std::abs(std::sin(glfwGetTime())));

        glUseProgram(shader_program_1.GetId());
        GLint offset_location = glGetUniformLocation(shader_program_1.GetId(), "offset");
        glUniform3f(offset_location, offset, 0.0f, 0.0f);
        glBindVertexArray(VAO_1.GetId());
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glUseProgram(shader_program_2.GetId());
        GLint color_location = glGetUniformLocation(shader_program_2.GetId(), "uniColor");
        glUniform4f(color_location, 0.0f, 0.0f, pulse, 1.0f);

        glBindVertexArray(VAO_2.GetId());
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
