#ifndef GLOBALS_H
#define GLOBALS_H

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "../components/physics/physics.h"

int g_gl_width = 1280;       //constants
int g_gl_height = 720;       //constants
GLFWwindow *g_window = NULL; //main

// camera
// glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 0.0f);
// glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
// glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 cameraPos = glm::vec3(-5.22f, 1.75f, -4.6f);
glm::vec3 cameraFront = glm::vec3(0.94f, -0.33f, -0.09f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

bool firstMouse = true;
// float yaw = -90.0f; // yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
// float pitch = 0.0f;
float yaw = 1.6f; // yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch = -19.6f;

// float lastX = g_gl_width / 2.0;
// float lastY = g_gl_height / 2.0;
float lastX = 1373.54;
float lastY = 607.734;

float fov = 45.0f;

double vx = 0.0;
double vy = 0.0;
double vz = 0.0;

// timing
float deltaTime = 0.0f; // time between current frame and last frame
float lastFrame = 0.0f;

int maxrand = 10;

GLuint shader_programme;

int proj_mat_location;
int view_mat_location;

glm::mat4 view;
glm::mat4 projection;

Bullet *world;

#endif