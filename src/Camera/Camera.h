#pragma once 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera 
{
public:
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;

    float yaw;
    float pitch;

    float fov;
    float aspect;
    float near;
    float far;

    Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch);
    ~Camera();

    glm::mat4 getViewMatrix();
    glm::mat4 getProjectionMatrix();

    void setAspect(float aspect);
    void processKeyboardMovement(float dx, float dy, float dz);
    void processMouseMovement(float xoffset, float yoffset);
};
