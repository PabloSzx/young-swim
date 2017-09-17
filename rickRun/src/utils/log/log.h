#ifndef LOG_H
#define LOG_H

#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h>

bool log_restart_gl_log();
bool log_gl_log(const char *message, ...);

bool log_gl_log_err(const char *message, ...);

void log_print_shader_info_log(GLuint shader_index);

#endif