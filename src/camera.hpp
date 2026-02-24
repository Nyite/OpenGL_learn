#pragma once
#include "glm/ext/matrix_transform.hpp"
#include "glm/geometric.hpp"
#include "glm/trigonometric.hpp"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class TCamera {
  public:
    TCamera(glm::vec3 init_pos) : pos(init_pos) {
    }

    void OnMouseMovement(double xpos, double ypos, float mouse_sence) {
        static double last_xpos = xpos;
        static double last_ypos = ypos;

        double xdiff = xpos - last_xpos;
        double ydiff = last_ypos - ypos;
        last_xpos = xpos;
        last_ypos = ypos;

        yaw += static_cast<float>(xdiff) * mouse_sence;
        pitch += static_cast<float>(ydiff) * mouse_sence;

        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;

        glm::vec3 new_direction;
        new_direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        new_direction.y = sin(glm::radians(pitch));
        new_direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction = glm::normalize(new_direction);
    }

    void OnPollEvent(GLFWwindow* window, float camera_speed) {
        float effective_camera_speed = camera_speed;
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
            effective_camera_speed *= 2;
        }

        glm::vec3 camera_right = glm::normalize(glm::cross(direction, {0.0f, 1.0f, 0.0f}));
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            pos += effective_camera_speed * direction;
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            pos -= effective_camera_speed * direction;
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            pos -= effective_camera_speed * camera_right;
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            pos += effective_camera_speed * camera_right;
        }
    }

    glm::mat4 GetViewMatrix() {
        return glm::lookAt(pos, pos + direction, {0.0f, 1.0f, 0.0f});
    }

  private:
    glm::vec3 pos;
    glm::vec3 direction;

    float yaw = -90.0f;
    float pitch = 0.0f;
};
