#include <stdarg.h> // used by log functions to have variable number of args
#include <stdio.h>
#include <time.h>
#include <GL/glew.h>    // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library

#include "../../data/constants.h"
#include "log.h"

bool restart_gl_log()
{
    FILE *file = fopen(GL_LOG_FILE, "w");
    if (!file)
    {
        fprintf(
            stderr,
            "ERROR: could not open GL_LOG_FILE log file %s for writing\n",
            GL_LOG_FILE);
        return false;
    }
    time_t now = time(NULL);
    char *date = ctime(&now);
    fprintf(file, "GL_LOG_FILE log. local time %s\n", date);
    fclose(file);
    return true;
}

bool gl_log(const char *message, ...)
{
    va_list argptr;
    FILE *file = fopen(GL_LOG_FILE, "a");
    if (!file)
    {
        fprintf(
            stderr,
            "ERROR: could not open GL_LOG_FILE %s file for appending\n",
            GL_LOG_FILE);
        return false;
    }
    va_start(argptr, message);
    vfprintf(file, message, argptr);
    va_end(argptr);
    fclose(file);
    return true;
}

/* same as gl_log except also prints to stderr */
bool gl_log_err(const char *message, ...)
{
    va_list argptr;
    FILE *file = fopen(GL_LOG_FILE, "a");
    if (!file)
    {
        fprintf(
            stderr,
            "ERROR: could not open GL_LOG_FILE %s file for appending\n",
            GL_LOG_FILE);
        return false;
    }
    va_start(argptr, message);
    vfprintf(file, message, argptr);
    va_end(argptr);
    va_start(argptr, message);
    vfprintf(stderr, message, argptr);
    va_end(argptr);
    fclose(file);
    return true;
}

void print_shader_info_log(GLuint shader_index)
{
    int max_length = 2048;
    int actual_length = 0;
    char log[2048];
    glGetShaderInfoLog(shader_index, max_length, &actual_length, log);
    printf("shader info log for GL index %i:\n%s\n", shader_index, log);
    gl_log("shader info log for GL index %i:\n%s\n", shader_index, log);
}

void print_programme_info_log(GLuint sp)
{
    int max_length = 2048;
    int actual_length = 0;
    char log[2048];
    glGetProgramInfoLog(sp, max_length, &actual_length, log);
    printf("program info log for GL index %u:\n%s", sp, log);
    gl_log("program info log for GL index %u:\n%s", sp, log);
}

void glfw_error_callback(int error, const char *description) {
    fputs(description, stderr);
    gl_log_err("%s\n", description);
}
