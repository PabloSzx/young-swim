#ifndef CAMERA_H
#define CAMERA_H

#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../../containers/world/world.hpp"

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
extern World* core;

extern GLuint shader_programme_cube;
extern GLuint shader_programme;

extern int proj_mat_location;
extern int view_mat_location;
extern glm::mat4 view;
extern glm::mat4 projection;

void camera_viewMatrixLocation();
void camera_projMatrixLocation();
void camera_viewMatrixLocation(GLuint shaderprog);
void camera_projMatrixLocation(GLuint shaderprog);

void camera_viewMatrixPerspective(glm::vec3 charPos);
void camera_projectionMatrixPerspective();

void camera_viewProjUpdate();
// void camera_resetPerspective();

#endif
