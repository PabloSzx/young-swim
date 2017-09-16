#include "log.h"
#include <stdio.h>
#include <GLFW/glfw3.h>
#include <time.h>
#include <assert.h>
#include <stdarg.h>
#include "../../data/constants.h"

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
