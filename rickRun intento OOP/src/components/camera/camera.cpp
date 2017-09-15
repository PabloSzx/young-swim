#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>    // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library

// #include "../../utils/window/window.h"
#include "../../utils/gl_utils/gl_utils.h"
#include "../../data/constants.h"

#include "camera.h"

Camera::Camera() {
    this->cameraPos = glm::vec3(0.0f, 0.0f, 0.0f);
    this->cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    this->cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

    this->firstMouse = true;
    this->yaw = -90.0f;
    this->pitch = 0.0f;

    // this->lastX = 0;
    // this->lastY = 0;
    // float asd = wndow->width / 2.0;
    this->lastX = 1280 / 2.0;
    this->lastY = 720 / 2.0;
    this->fov = 45.0f;

    // timing
    this->deltaTime = 0.0f;
    this->lastFrame = 0.0f;

    this->projection = glm::perspective(glm::radians(this->fov), (float)1280 / (float)720, 0.1f, 100.0f);
    this->view = glm::lookAt(this->cameraPos, this->cameraPos + this->cameraFront, this->cameraUp);
}

void use_view_mat_location() {
    int view_mat_location = glGetUniformLocation(engine->shader_programme, "view");
    camera->view_mat_location = view_mat_location;
    glUseProgram(engine->shader_programme);
    glUniformMatrix4fv(camera->view_mat_location, 1, GL_FALSE, &camera->view[0][0]);
}

void use_proj_mat_location()
{
    int proj_mat_location = glGetUniformLocation(engine->shader_programme, "proj");
    camera->proj_mat_location = proj_mat_location;
    glUseProgram(engine->shader_programme);
    glUniformMatrix4fv(camera->proj_mat_location, 1, GL_FALSE, &camera->projection[0][0]);
}

void lookAt() {
    camera->view = glm::lookAt(camera->cameraPos, camera->cameraPos + camera->cameraFront, camera->cameraUp);
    glUniformMatrix4fv(camera->view_mat_location, 1, GL_FALSE, &camera->view[0][0]);
}
