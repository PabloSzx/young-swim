#ifndef INPUT_H
#define INPUT_H

#ifdef APPLE
#include <BulletDynamics/btBulletDynamicsCommon.h>
#else
#include <btBulletDynamicsCommon.h>
#endif


#include "../sound/sound.hpp"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include "../physics/physics.hpp"
#include "../../containers/world/world.hpp"

extern sound *snd_01;
extern sound *snd_02;

extern glm::vec3 sun;

extern sound **background;

extern World *core;

extern int g_gl_width;
extern int g_gl_height;
extern GLFWwindow *g_window;
extern glm::vec3 cameraPos;
extern glm::vec3 cameraFront;
extern glm::vec3 cameraUp;
extern bool firstMouse;
extern bool mouseIn;
extern float yaw;
extern float pitch;
extern float lastX;
extern float lastY;
extern float fov;
extern float deltaTime;
extern float lastFrame;
extern double vx;
extern double vy;
extern double vz;
extern bool allowJump;
extern double jumpHorizontalForce;
extern double jumpVerticalUpForce;
extern double jumpVerticalDownForce;
extern double jumpForwardForce;
extern double jumpBackwardForce;
extern double fps;

extern Bullet* platformWorld;

void input_setCallbacks();
void input_framebuffer_size_callback(GLFWwindow *window, int width, int height);
void input_mouse_callback(GLFWwindow *window, double xpos, double ypos);
void input_scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void input_processInput(GLFWwindow *window);

#endif
