#ifndef CAMERA_H
#define CAMERA_H

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

extern int g_gl_width;
extern int g_gl_height;
extern GLFWwindow *g_window;
extern glm::vec3 cameraPos;
extern glm::vec3 cameraFront;
extern glm::vec3 cameraUp;
extern bool firstMouse;
extern float yaw;
extern float pitch;
extern float lastX;
extern float lastY;
extern float fov;
extern float deltaTime;
extern float lastFrame;

extern GLuint shader_programme;

extern int proj_mat_location;
extern int view_mat_location;
extern glm::mat4 view;
extern glm::mat4 projection;

void viewMatrixLocation();
void projMatrixLocation();
void viewMatrixPerspective();
void projectionMatrixPerspective();

#endif
