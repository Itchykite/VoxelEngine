#include "Camera.h"
#include <algorithm>

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
    : Position(position), Up(up), yaw(yaw), pitch(pitch), Front(glm::vec3(0, 0, -1)), fov(45.0f), aspect(4.0f/3.0f), near(0.1f), far(100.0f)
{
    float radYaw = glm::radians(yaw);
    float radPitch = glm::radians(pitch);
    Front.x = cos(radYaw) * cos(radPitch);
    Front.y = sin(radPitch);
    Front.z = sin(radYaw) * cos(radPitch);
    Front = glm::normalize(Front);
}

Camera::~Camera() {}

glm::mat4 Camera::getViewMatrix() 
{
    return glm::lookAt(Position, Position + Front, Up);
}

glm::mat4 Camera::getProjectionMatrix() 
{
    return glm::perspective(glm::radians(fov), aspect, near, far);
}

void Camera::setAspect(float aspect) 
{
    this->aspect = aspect;
}

void Camera::processKeyboardMovement(float dx, float dy, float dz)
{
    // WASD style movement
    glm::vec3 right = glm::normalize(glm::cross(Front, Up));
    Position += Front * dz;
    Position += right * dx;
    Position += Up * dy;
}

void Camera::processMouseMovement(float xoffset, float yoffset)
{
    float sensitivity = 0.5f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw   += xoffset;
    pitch += yoffset;

    pitch = std::clamp(pitch, -89.0f, 89.0f);

    float radYaw = glm::radians(yaw);
    float radPitch = glm::radians(pitch);
    Front.x = cos(radYaw) * cos(radPitch);
    Front.y = sin(radPitch);
    Front.z = sin(radYaw) * cos(radPitch);
    Front = glm::normalize(Front);
}
