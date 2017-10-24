#ifndef GLOBALS_H
#define GLOBALS_H

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "../components/physics/physics.hpp"
#include "../containers/world/world.hpp"

int g_gl_width = 1280;       //constants
int g_gl_height = 720;       //constants
GLFWwindow *g_window = NULL; //main

glm::vec3 sun = glm::vec3(0.7f, -12.2f, 2.3f);
// Sounds
// sound *snd_01 = new sound((const char*)"audio/RICK_RUN2.wav");
// sound *snd_02 = new sound((const char*)"audio/RICK_RUN1.wav");

sound **background = static_cast<sound **>(malloc(sizeof(sound *) * 6));;;

World *core;
// camera
// glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 0.0f);
// glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
// glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 cameraPos = glm::vec3(-14.4f, 2.52f, -4.3f);
glm::vec3 cameraFront = glm::vec3(1.0f, 0.0f, 0.0f);
glm::vec3 cameraFrontInit = glm::vec3(1.0f, 0.0f, 0.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

bool mouseIn = false;

bool allowJump = true;
bool allowDownJump = true;
double jumpHorizontalForce;
double jumpVerticalUpForce;
double jumpVerticalDownForce;
double jumpForwardForce;
double jumpBackwardForce;

bool restart;
bool fullscreen = true;

bool firstMouse = true;
// float yaw = -90.0f; // yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
// float pitch = 0.0f;
float yaw = 0; // yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch = -20.0f;

float lastX = g_gl_width / 2.0;
float lastY = g_gl_height / 2.0;
// float lastX = 1373.54;
// float lastY = 607.734;

float fov = 45.0f;

double vx = 0.0;
double vy = 0.0;
double vz = 0.0;

double fps = 60.0;

// timing
float deltaTime = 0.0f; // time between current frame and last frame
float lastFrame = 0.0f;

int maxrand = 10;

GLuint shader_programme;

GLuint color;
GLuint sunLocation;
int proj_mat_location;
int view_mat_location;


glm::mat4 view;
glm::mat4 projection;

Bullet *platformWorld; //MUNDO DE RICK Y SUS PLATAFORMAS... AQUI RICK RECIBE GRAVEDAD Y LAS PLATAFORMAS SE MUEVEN XD
Bullet *parallaxHouses;
Bullet *parallaxProps;
Bullet *distanceScore;
#endif
