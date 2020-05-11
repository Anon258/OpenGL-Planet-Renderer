#include "Camera.hpp"

void Camera::UpdateCameraVectors(){
    glm::vec3 direction;
    direction.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    direction.y = sin(glm::radians(Pitch));
    direction.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));

    camFront = glm::normalize(direction);
    camRight = glm::normalize(glm::cross(camFront, worldUp));
    camUp = glm::normalize(glm::cross(camRight, camFront));
}

Camera::Camera(){
    camPos = glm::vec3(0.0,0.0,13.0);
    camFront =  glm::vec3(0.0,0.0,-1.0);
    camUp = glm::vec3(0.0,1.0,0.0);
    worldUp = glm::vec3(0.0,1.0,0.0);
    
    Yaw = -90.0f;
    Pitch = 0.0f;
    
    sensitivity = 0.1f;
    speed = 2.0f;
    
    UpdateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix(){
    return glm::lookAt(camPos, camPos + camFront, camUp);
}

glm::vec3 Camera::GetCamPos(){
    return camPos;
}

glm::vec3 Camera::GetCamFront(){
    return camFront;
}

void Camera::MouseMovement(float xoffset, float yoffset){
    xoffset *= sensitivity;
    yoffset *= sensitivity;
    
    Yaw   += xoffset;
    Pitch += yoffset;
    
    if(Pitch > 89.0f)
        Pitch = 89.0f;
    if(Pitch < -89.0f)
        Pitch = -89.0f;
    
    UpdateCameraVectors();
}

void Camera::KeyboardInput(Cam_mov direction, float deltaTime){
    
    float velocity = speed * deltaTime;
    if(direction == FORWARD)
        camPos += camFront * velocity;
    else if(direction == BACKWARD)
        camPos -= camFront * velocity;
    else if(direction == RIGHT)
        camPos += camRight * velocity;
    else if(direction == LEFT)
        camPos -= camRight * velocity;
    else if(direction == UP)
        camPos += worldUp * velocity;
    else if(direction == DOWN)
        camPos -= worldUp * velocity;
}
