#ifndef WINDOW_H
#define WINDOW_H

#include <GLFW/glfw3.h>

extern int g_gl_width;
extern int g_gl_height;
extern GLFWwindow *g_window;
extern float deltaTime;
extern float lastFrame;

bool window_start_gl();
void window_flags();
void _update_fps_counter(GLFWwindow* window);
void glfw_error_callback(int error, const char *description);
void glfw_window_size_callback(GLFWwindow *window, int width, int height);
void frameCounter();
#endif
