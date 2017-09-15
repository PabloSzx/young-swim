#include <stdio.h>
#include <GL/glew.h>    // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library
#include "../log/log.h"

#include "../../data/global.h"

#include "./window.h"

Window::Window(int width, int height) {
    this->width = width;
    this->height = height;

    // start_gl(window->window);
    // flags();


    // glfwSetFramebufferSizeCallback(this->window, this->framebuffer_size_callback);
    // glfwSetCursorPosCallback(this->window, this->mouse_callback);
    // glfwSetScrollCallback(this->window, this->scroll_callback);
    
    this->deltaTime = 0.0f;
    this->lastFrame = 0.0f;
}

// void glfw_window_size_callback(GLFWwindow *window, int width, int height)
// {
//     this->width = width;
//     this->height = height;
//     printf("width %i height %i\n", width, height);
//     /* update any perspective matrices used here */
// }


void _update_fps_counter(GLFWwindow *wndow) {
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
        glfwSetWindowTitle(wndow, tmp);
        frame_count = 0;
    }
    frame_count++;
}

bool start_gl() {
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
    
    /*GLFWmonitor* mon = glfwGetPrimaryMonitor ();
	const GLFWvidmode* vmode = glfwGetVideoMode (mon);
	g_window = glfwCreateWindow (
		vmode->width, vmode->height, "Extended GL Init", mon, NULL
	);*/
    
    window = glfwCreateWindow(
        // g_gl_width, g_gl_height, "Extended Init.", glfwGetPrimaryMonitor(), NULL
        wndow->width, wndow->height, "Extended Init.", NULL, NULL
    );
    if (!window)
    {
        fprintf(stderr, "ERROR: could not open window with GLFW3\n");
        glfwTerminate();
        return false;
    }

    // glfwSetWindowSizeCallback(window->window, glfw_window_size_callback);

    glfwMakeContextCurrent(window);
    
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

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
// void framebuffer_size_callback(GLFWwindow *window, int width, int height)
// {
//     // make sure the viewport matches the new window dimensions; note that width and
//     // height will be significantly larger than specified on retina displays.
//     glViewport(0, 0, width, height);
// }

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void flags() {
    glEnable(GL_DEPTH_TEST);              // enable depth-testing
    glDepthFunc(GL_LESS);                 // depth-testing interprets a smaller value as "closer"
    glEnable(GL_CULL_FACE);               // cull face
    glCullFace(GL_BACK);                  // cull back face
    glFrontFace(GL_CCW);                  // set counter-clock-wise vertex order to mean the front
    glClearColor(0.8f, 0.5f, 0.5f, 1.0f); // grey background to help spot mistakes
    glViewport(0, 0, wndow->width, wndow->height);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

// void glfw_window_size_callback(GLFWwindow *wndow, int width, int height)
// {
//     window->width = width;
//     window->height = height;
//     printf("width %i height %i\n", width, height);
//     /* update any perspective matrices used here */
// }
