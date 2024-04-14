#include "Camera.hpp"

void Camera::updateCameraVectors()
{
    glm::vec3 front = glm::vec3(0.0f);
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(front);
    Right = glm::normalize(glm::cross(Front, WorldUp));  
    Up = glm::normalize(glm::cross(Right, Front));
}

void Camera::move(Movement movement, float dt)
{
    float velocity = MovementSpeed * dt;

    if (movement == FORWARD) Position += Front * velocity;
    if (movement == BACKWARD) Position -= Front * velocity;
    if (movement == LEFT) Position -= Right * velocity;
    if (movement == RIGHT) Position += Right * velocity;
}

void Camera::rotate(float xoffset, float yoffset, GLboolean constrainPitch)
{
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    Yaw += xoffset;
    Pitch += yoffset;

    if (constrainPitch)
    {
        if (Pitch > 89.0f)
            Pitch = 89.0f;
        if (Pitch < -89.0f)
            Pitch = -89.0f;
    }

    updateCameraVectors();
}

void Camera::zoom(float yoffset)
{
    Zoom -= (float)yoffset;
    if (Zoom < 1.0f)
        Zoom = 1.0f;
    if (Zoom > 45.0f)
        Zoom = 45.0f;
}

glm::mat4 Camera::GetViewMatrix()
{
    return glm::lookAt(Position, Position + Front, Up);
}
