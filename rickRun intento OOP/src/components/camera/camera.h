#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

// #include "../../utils/window/window.h"
// #include "../../utils/gl_utils/gl_utils.h"

class Camera{
private:
        
public:
    Camera();
    glm::vec3 cameraPos;
    glm::vec3 cameraFront;
    glm::vec3 cameraUp;

    glm::mat4 projection;
    glm::mat4 view;

    int view_mat_location;
    int proj_mat_location;

    bool firstMouse;
    float yaw; // yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
    float pitch;

    float lastX;
    float lastY;
    float fov;

    float deltaTime;
    float lastFrame;
};

void use_view_mat_location();
void use_proj_mat_location();
void lookAt();

extern Camera* camera;

#endif