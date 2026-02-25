#include <iostream>

#include <fmt/core.h>
#include <fmt/format.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "camera.hpp"
#include "objects/cube.hpp"
#include "objects/light.hpp"
#include "objects/material.hpp"
#include "shaders/shader.hpp"
#include "shaders/program.hpp"
#include "textures/texture.hpp"
#include "camera.hpp"

namespace {
constexpr size_t InitWindowWigth = 800z;
constexpr size_t InitWindowHeight = 600z;
float AspectRatio = static_cast<float>(InitWindowWigth) / InitWindowHeight;

TCamera camera({0.0f, 0.0f, 3.0f});

constexpr float mouse_sence = 0.1f;
constexpr float camera_speed = 0.01f;
} // namespace

void framebufferSizeCallback(GLFWwindow*, int width, int height) {
    glViewport(0, 0, width, height);
    AspectRatio = static_cast<float>(width) / height;
}

static void cursor_position_callback([[maybe_unused]] GLFWwindow* window, double xpos, double ypos) {
    camera.OnMouseMovement(xpos, ypos, mouse_sence);
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

    camera.OnPollEvent(window, camera_speed);
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

    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    TMaterial cube_material{.diffuse_map = TTexture("assets/textures/container2.png", GL_RGBA, GL_TEXTURE0),
                            .specular_map = TTexture("assets/textures/container2_specular.png", GL_RGBA, GL_TEXTURE1),
                            .shine = 64.0f};

    TShader<EShaderVariant::Vertex> vertex_shader("assets/shaders/vertex_shader.glsl");
    TShader<EShaderVariant::Fragment> fragment_shader("assets/shaders/fragment_shader.glsl");
    TShaderProgram cube_shader(vertex_shader, fragment_shader);
    cube_shader.SetMaterial(cube_material);

    TCube cube_object(cube_shader);

    TShader<EShaderVariant::Vertex> light_vsh("assets/shaders/light_source_vertex.glsl");
    TShader<EShaderVariant::Fragment> light_fsh("assets/shaders/light_source_fragment.glsl");
    TShaderProgram light_shp(light_vsh, light_fsh);
    light_shp.Use();
    light_shp.SetUnifiorm("color", 1.0f, 1.0f, 1.0f);

    TCube light_object(light_shp);

    glm::vec3 init_light_pos{0.0f, 1.0f, 1.0f};
    TLight light{.ambient = glm::vec3(0.1f),
                 .diffuse = glm::vec3(0.9f),
                 .specular = glm::vec3(1.0f),
                 .position = init_light_pos};

    glEnable(GL_DEPTH_TEST);
    while (!glfwWindowShouldClose(window)) {
        processEvents(window);

        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 projection = glm::perspective(glm::radians(75.0f), AspectRatio, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();

        float time = static_cast<float>(glfwGetTime());
        glm::mat4 light_model(1.0f);
        light_model = glm::translate(light_model, init_light_pos + glm::vec3{2.0f, 0.0f, 0.0f} * sin(time));
        light.position = light_model * glm::vec4(init_light_pos, 1.0f);

        cube_shader.SetLight(light);
        cube_object.Draw(projection, view);

        light_object.modelMx = glm::scale(light_model, {0.2f, 0.2f, 0.2f});
        light_object.Draw(projection, view);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
