#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string>
#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <assimp/cimport.h> // C importer
#include <assimp/scene.h> // collects data
#include <assimp/postprocess.h> // various extra operations
#include "../shader/shader.hpp"
#include "../log/log.hpp"
#include "window.hpp"

using namespace std;

bool window_start_gl() {
    log_gl_log("starting GLFW %s", glfwGetVersionString());
    
    glfwSetErrorCallback(window_glfw_error_callback);
    if (!glfwInit())
    {
        fprintf(stderr, "ERROR: could not start GLFW3\n");
        return false;
    }
    
    if (fullscreen) {
        g_window = glfwCreateWindow(
            g_gl_width, g_gl_height, "Extended Init.", glfwGetPrimaryMonitor(), NULL
            
        );
    } else {
        g_window = glfwCreateWindow(
            g_gl_width, g_gl_height, "Extended Init.", NULL, NULL
            
        ); 
    }
    if (!g_window)
    {
        fprintf(stderr, "ERROR: could not open window with GLFW3\n");
        glfwTerminate();
        return false;
    }
    
    glfwSetWindowSizeCallback(g_window, window_glfw_window_size_callback);
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
    log_gl_log("renderer: %s\nversion: %s\n", renderer, version);
    
    return true;
}

void window_flags() {
    glEnable(GL_DEPTH_TEST); // enable depth-testing
    glDepthFunc(GL_LESS);    // depth-testing interprets a smaller value as "closer"
    // glEnable (GL_CULL_FACE); // cull face
    glCullFace(GL_BACK);                  // cull back face
    glFrontFace(GL_CCW);                  // set counter-clock-wise vertex order to mean the front
    glClearColor(0.8f, 0.5f, 0.5f, 1.0f); // grey background to help spot mistakes
    glViewport (0, 0, g_gl_width, g_gl_height);
    
    glfwSetInputMode(g_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void window_update_fps_counter(GLFWwindow *window)
{
    cout << "--";
    static double previous_seconds = glfwGetTime();
    cout << "previous: " << previous_seconds;
    static int frame_count;
    double current_seconds = glfwGetTime();
    cout << "  current: " << current_seconds << "  --" << endl;
    double elapsed_seconds = current_seconds - previous_seconds;
    if (elapsed_seconds > 0.25)
    {
        previous_seconds = current_seconds;
        fps = (double)frame_count / elapsed_seconds;
        char tmp[128];
        sprintf(tmp, "opengl @ fps: %.2f", fps);
        glfwSetWindowTitle(window, tmp);
        frame_count = 0;
    }

    // cout << "elapsed: " << elapsed_seconds << endl;
    frame_count++;
}

void window_glfw_error_callback(int error, const char *description)
{
    fputs(description, stderr);
    log_gl_log_err("%s\n", description);
}
// a call-back function
void window_glfw_window_size_callback(GLFWwindow *window, int width, int height)
{
    g_gl_width = width;
    g_gl_height = height;
    printf("width %i height %i\n", width, height);
    /* update any perspective matrices used here */
}

void window_frameCounter()
{
    // cout << "frameCounter" << endl;
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
}

void window_clear() {
    glClearColor(0.710f, 0.882f, 0.917f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void window_swap() {
    glfwSwapBuffers(g_window);
    glfwPollEvents();
}

void window_calculateFps() {
    static const int NUM_SAMPLES = 10;
    static float frameTimes[NUM_SAMPLES];
    static int currentFrame = 0;

    static float prevTicks = glfwGetTime();
    float currentTicks = glfwGetTime();
    // cout << "prev: " << prevTicks;
    // cout << "  current: " << currentTicks << endl;

    
    _frameTime = currentTicks - prevTicks;
    frameTimes[currentFrame % NUM_SAMPLES] = _frameTime;

    prevTicks = currentTicks;
    int count;
    currentFrame++;

    if (currentFrame < NUM_SAMPLES) {
        count = currentFrame;
    } else {
        count = NUM_SAMPLES;
    }

    float frameTimeAverage = 0;
    for (int i = 0; i < count; i++) {
        frameTimeAverage += frameTimes[i];
    }
    frameTimeAverage /= count;

    if (frameTimeAverage > 0) {
        _fps = frameTimeAverage;
    } else {
        _fps = 1/60.0f;
    }


}
