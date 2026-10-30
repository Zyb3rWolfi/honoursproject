#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Free-fly camera. Pure data + matrix maths; input handling lives elsewhere.
class Camera {
public:
    glm::vec3 Position{0.0f, 2.0f, 6.0f};
    float Yaw   = -90.0f;    // degrees, -90 looks down -Z
    float Pitch = -15.0f;    // degrees
    float Fov   = 60.0f;     // vertical, degrees
    float Near  = 0.1f;
    float Far   = 1000.0f;

    glm::vec3 Forward() const {
        const float yaw   = glm::radians(Yaw);
        const float pitch = glm::radians(Pitch);
        return glm::normalize(glm::vec3(std::cos(yaw) * std::cos(pitch),
                                        std::sin(pitch),
                                        std::sin(yaw) * std::cos(pitch)));
    }

    glm::vec3 Right() const {
        return glm::normalize(glm::cross(Forward(), glm::vec3(0.0f, 1.0f, 0.0f)));
    }

    glm::mat4 View() const {
        return glm::lookAt(Position, Position + Forward(), glm::vec3(0.0f, 1.0f, 0.0f));
    }

    glm::mat4 Projection(float aspect) const {
        return glm::perspective(glm::radians(Fov), aspect, Near, Far);
    }
};