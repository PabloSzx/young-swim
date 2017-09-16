#ifndef LOG_H
#define LOG_H

#include <GLFW/glfw3.h>

bool restart_gl_log();
bool gl_log(const char *message, ...);

bool gl_log_err(const char *message, ...);

void print_shader_info_log(GLuint shader_index);

#endif