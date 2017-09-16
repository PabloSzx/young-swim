#ifndef SHADER_H
#define SHADER_H

#include <stdarg.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

extern int g_gl_width;
extern int g_gl_height;
extern GLFWwindow* g_window;

bool parse_file_into_str (const char* file_name, char* shader_str, int max_len);
bool create_shader (const char* file_name, GLuint* shader, GLenum type);
bool is_programme_valid (GLuint sp);
bool create_programme (GLuint vert, GLuint frag, GLuint* programme);
GLuint create_programme_from_files ();

#endif
