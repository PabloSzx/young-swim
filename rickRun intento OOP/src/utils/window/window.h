#ifndef WINDOW_H
#define WINDOW_H

#include <GLFW/glfw3.h>

// #include "../../components/camera/camera.h"

class Window{
private:
  
public:
  Window(int width, int height);
  GLFWwindow *window;
  int height;
  int width;
  // timing
  float deltaTime; // time between current frame and last frame
  float lastFrame;

};

bool start_gl();
// static void processInput(Camera* camera);
void flags();
// static void glfw_window_size_callback(GLFWwindow *window, int width, int height);
void _update_fps_counter(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
// void glfw_window_size_callback(GLFWwindow *window, int width, int height);

extern Window *wndow;
extern GLFWwindow *window;

#endif

