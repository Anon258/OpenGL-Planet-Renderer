#ifndef Camera_hpp
#define Camera_hpp

#include <GL/glew.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

enum Cam_mov {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

class Camera{
    glm::vec3 camPos;
    glm::vec3 camFront;
    glm::vec3 camUp;
    glm::vec3 camRight;
    glm::vec3 worldUp;
    
    GLfloat Yaw, Pitch;
    
    float sensitivity, speed;
public:
    void UpdateCameraVectors();
    Camera();
    glm::mat4 GetViewMatrix();
    glm::vec3 GetCamPos();
    void MouseMovement(float xoffset, float yoffset);
    void KeyboardInput(Cam_mov direction, float deltaTime);
};
#endif /* Camera_hpp */
