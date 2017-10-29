#ifndef SHADER_H
#define SHADER_H

#include <stdarg.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

extern int g_gl_width;
extern int g_gl_height;
extern GLFWwindow* g_window;

bool shader_parse_file_into_str (const char* file_name, char* shader_str, int max_len);
bool shader_create_shader(const char *file_name, GLuint *shader, GLenum type);
bool shader_is_programme_valid(GLuint sp);
bool shader_create_programme(GLuint vert, GLuint frag, GLuint *programme);
GLuint shader_create_programme_from_files();
GLuint shader_create_programme_from_files(const char* vs, const char* fs);

#endif
