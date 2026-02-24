#include <ostream>
#include <iostream>

#include <fmt/core.h>
#include <fmt/format.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/geometric.hpp"
#include "glm/trigonometric.hpp"
#include "shaders/shader.hpp"
#include "shaders/program.hpp"
#include "buffers/buffer_object.hpp"
#include "buffers/vertex_array.hpp"
#include "textures/texture.hpp"
#include "shapes.hpp"

namespace {
    constexpr size_t InitWindowWigth = 800z;
    constexpr size_t InitWindowHeight = 600z;
    float AspectRatio = static_cast<float>(InitWindowWigth) / InitWindowHeight;

    glm::vec3 camera_pos(0.0f, 0.0f, 3.0f);
    glm::vec3 camera_direction(0.0f, 0.0f, -1.0f);

    float yaw = -90.0f;
    float pitch = 0.0f;

    constexpr float mouse_sence = 0.1f;
    constexpr float camera_speed = 0.01f;
}

void framebufferSizeCallback(GLFWwindow*, int width, int height) {
    glViewport(0, 0, width, height);
    AspectRatio = static_cast<float>(width) / height;
}

static void cursor_position_callback([[maybe_unused]] GLFWwindow* window, double xpos, double ypos) {
    static double last_xpos = static_cast<double>(InitWindowWigth) / 2.0;
    static double last_ypos = static_cast<double>(InitWindowHeight) / 2.0;

    double xdiff = xpos - last_xpos;
    double ydiff = last_ypos - ypos;
    last_xpos = xpos;
    last_ypos = ypos;

    yaw += static_cast<float>(xdiff) * mouse_sence;
    pitch += static_cast<float>(ydiff) * mouse_sence;

    if(pitch > 89.0f)
        pitch = 89.0f;
    if(pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    camera_direction = glm::normalize(direction);
}

void processEvents(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    float effective_camera_speed = camera_speed;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        effective_camera_speed *= 2;
    }

    glm::vec3 camera_right = glm::normalize(glm::cross(camera_direction, {0.0f, 1.0f, 0.0f}));
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera_pos += effective_camera_speed * camera_direction;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera_pos -= effective_camera_speed * camera_direction;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera_pos -= effective_camera_speed * camera_right;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera_pos += effective_camera_speed * camera_right;
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
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    TVertextArray VAO;
    VAO.Bind();

    TBufferObject<EBufferVariant::Vertex> VBO(std::span{cubeVerticies});
    VBO.Bind();

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    TShader<EShaderVariant::Vertex> vertex_shader("assets/shaders/vertex_shader.glsl");
    TShader<EShaderVariant::Fragment> fragment_shader("assets/shaders/fragment_shader.glsl");
    TShaderProgram shader_program(vertex_shader, fragment_shader);

    shader_program.Use();
    TTexture texture_1("assets/textures/container.jpg", GL_RGB, GL_TEXTURE0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    shader_program.SetUnifiorm("tex1", 0);

    TTexture texture_2("assets/textures/awesomeface.png", GL_RGBA, GL_TEXTURE1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    shader_program.SetUnifiorm("tex2", 1);

    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f), 
        glm::vec3( 2.0f,  5.0f, -15.0f), 
        glm::vec3(-1.5f, -2.2f, -2.5f),  
        glm::vec3(-3.8f, -2.0f, -12.3f),  
        glm::vec3( 2.4f, -0.4f, -3.5f),  
        glm::vec3(-1.7f,  3.0f, -7.5f),  
        glm::vec3( 1.3f, -2.0f, -2.5f),  
        glm::vec3( 1.5f,  2.0f, -2.5f), 
        glm::vec3( 1.5f,  0.2f, -1.5f), 
        glm::vec3(-1.3f,  1.0f, -1.5f)  
    };

    glEnable(GL_DEPTH_TEST);
    while (!glfwWindowShouldClose(window)) {
        processEvents(window);

        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 projection = glm::perspective(glm::radians(75.0f), AspectRatio, 0.1f, 100.0f);

        glm::mat4 view(1.0f);
        view = glm::lookAt(camera_pos, camera_pos + camera_direction, {0.0f, 1.0f, 0.0f});

        shader_program.Use();
        texture_1.Bind();
        texture_2.Bind();
        VAO.Bind();

        shader_program.SetUnifiorm("projection", glm::value_ptr(projection));
        shader_program.SetUnifiorm("view", glm::value_ptr(view));

        for (auto& pos : cubePositions) {
            glm::mat4 model(1.0f);
            model = glm::translate(model, pos);
            shader_program.SetUnifiorm("model", glm::value_ptr(model));
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
