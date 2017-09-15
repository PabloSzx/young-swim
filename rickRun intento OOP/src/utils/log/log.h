#ifndef LOG_H
#define LOG_H

#include <GLFW/glfw3.h> // GLFW helper library

class Log{
    
private:
    
public:
};

bool restart_gl_log();
bool gl_log(const char *message, ...);
bool gl_log_err(const char *message, ...);
void glfw_error_callback(int error, const char *description);

void print_shader_info_log(GLuint shader_index);
void print_programme_info_log(GLuint sp);

#endif