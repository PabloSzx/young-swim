#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string>
#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <assimp/cimport.h> // C importer
#include <assimp/scene.h> // collects data
#include <assimp/postprocess.h> // various extra operations
#include "../gl_utils/gl_utils.h"
#include "../log/log.h"
#include "window.h"

bool window_start_gl() {
    gl_log("starting GLFW %s", glfwGetVersionString());
    
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
    {
        fprintf(stderr, "ERROR: could not start GLFW3\n");
        return false;
    }
    
    /* We must specify 3.2 core if on Apple OS X -- other O/S can specify
	anything here. I defined 'APPLE' in the makefile for OS X */
    #ifdef APPLE
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #endif
    
    g_window = glfwCreateWindow(
        // g_gl_width, g_gl_height, "Extended Init.", glfwGetPrimaryMonitor(), NULL
        g_gl_width, g_gl_height, "Extended Init.", NULL, NULL
        
    );
    if (!g_window)
    {
        fprintf(stderr, "ERROR: could not open window with GLFW3\n");
        glfwTerminate();
        return false;
    }
    
    glfwSetWindowSizeCallback(g_window, glfw_window_size_callback);
    glfwMakeContextCurrent(g_window);
    
    glfwWindowHint(GLFW_SAMPLES, 4);
    
    // start GLEW extension handler
    glewExperimental = GL_TRUE;
    glewInit();
    
    // get version info
    const GLubyte *renderer = glGetString(GL_RENDERER); // get renderer string
    const GLubyte *version = glGetString(GL_VERSION);   // version as a string
    printf("Renderer: %s\n", renderer);
    printf("OpenGL version supported %s\n", version);
    gl_log("renderer: %s\nversion: %s\n", renderer, version);
    
    return true;
}

void window_flags() {
    glEnable(GL_DEPTH_TEST); // enable depth-testing
    glDepthFunc(GL_LESS);    // depth-testing interprets a smaller value as "closer"
    // glEnable (GL_CULL_FACE); // cull face
    glCullFace(GL_BACK);                  // cull back face
    glFrontFace(GL_CCW);                  // set counter-clock-wise vertex order to mean the front
    glClearColor(0.8f, 0.5f, 0.5f, 1.0f); // grey background to help spot mistakes
    // glViewport (0, 0, g_gl_width, g_gl_height);

    glfwSetInputMode(g_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void _update_fps_counter(GLFWwindow *window)
{
    static double previous_seconds = glfwGetTime();
    static int frame_count;
    double current_seconds = glfwGetTime();
    double elapsed_seconds = current_seconds - previous_seconds;
    if (elapsed_seconds > 0.25)
    {
        previous_seconds = current_seconds;
        double fps = (double)frame_count / elapsed_seconds;
        char tmp[128];
        sprintf(tmp, "opengl @ fps: %.2f", fps);
        glfwSetWindowTitle(window, tmp);
        frame_count = 0;
    }
    frame_count++;
}

void glfw_error_callback(int error, const char *description)
{
    fputs(description, stderr);
    gl_log_err("%s\n", description);
}
// a call-back function
void glfw_window_size_callback(GLFWwindow *window, int width, int height)
{
    g_gl_width = width;
    g_gl_height = height;
    printf("width %i height %i\n", width, height);
    /* update any perspective matrices used here */
}

void frameCounter() {
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
}
