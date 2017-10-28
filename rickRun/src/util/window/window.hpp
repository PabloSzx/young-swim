#ifndef WINDOW_H
#define WINDOW_H

#include <GLFW/glfw3.h>
#include <iostream>

extern int g_gl_width;
extern int g_gl_height;
extern GLFWwindow *g_window;
extern float deltaTime;
extern float lastFrame;
extern double fps;
extern bool fullscreen;
extern float _fps;
extern float _frameTime;

bool window_start_gl();
void window_flags();
void window_update_fps_counter(GLFWwindow *window);
void window_glfw_error_callback(int error, const char *description);
void window_glfw_window_size_callback(GLFWwindow *window, int width, int height);
void window_frameCounter();
void window_clear();
void window_swap();
void window_calculateFps();
#endif
