#include <iostream>
#include <GL/glew.h>    // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library
#include "./utils/window/window.h"
#include "./utils/log/log.h"
#include "./utils/gl_utils/gl_utils.h"
#include "./components/model/model.h"
// #include "./data/global.h"
#include "./components/camera/camera.h"
#include "./data/constants.h"

Camera *camera = NULL;
Window *wndow = new Window(1280, 720);
GLFWwindow* window = NULL;
GLUtils *engine = NULL;
GLuint shader_programme = 0;

// #include "./data/global.h"

void processInput(GLFWwindow *window);
void glfw_window_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);

    using namespace std;
int main()
{
    // Log* logs = new Log();
    // window = new Window(1280, 720);
    camera = new Camera();
    engine = new GLUtils();
    
    restart_gl_log();
    start_gl();
    
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    // glfwSetWindowSizeCallback(g_window, glfw_window_size_callback);
    
    shader_programme = create_programme_from_files();
    
    Model *axis = new Model(const_cast<char *>("mesh/axis.obj"));
    axis->setpos(glm::vec3(0.0f, 0.0f, 0.0f));
    axis->scale(glm::vec3(1.0f));
    axis->setmatloc(engine->shader_programme, "matrix");
    axis->model2shader(engine->shader_programme);

    Model *rick = new Model(const_cast<char *>("mesh/rick.obj"));
    rick->setpos(glm::vec3(0.0f, 0.0f, 0.0f));
    rick->scale(glm::vec3(1.0f));
    rick->setmatloc(engine->shader_programme, "matrix");
    rick->model2shader(engine->shader_programme);

    cout << rick->LX << " " << rick->LY << " " << rick->LZ << endl;
    
    float dx = 0.0f;
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        wndow->deltaTime = currentFrame - wndow->lastFrame;
        wndow->lastFrame = currentFrame;

        processInput(window);
        
        glClearColor(dx, 0.81640625f, 0.81640625f, 1.0f);
        // dx += 0.01f;
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(engine->shader_programme);
        
        use_proj_mat_location();

        // for (int i=0; i < 100; i++) {
        rick->setpos(glm::vec3(0, 1, 0));
        rick->setmatloc(engine->shader_programme, "matrix");
        rick->model2shader(engine->shader_programme);
        glBindVertexArray(rick->getvao());
        rick->model2shader(engine->shader_programme);
        glDrawArrays(GL_TRIANGLES, 0, rick->getnumvertices());
        // }

        axis->setpos(glm::vec3(0, 0, 0));
        axis->setmatloc(engine->shader_programme, "matrix");
        axis->model2shader(engine->shader_programme);
        glBindVertexArray(axis->getvao());
        axis->model2shader(engine->shader_programme);
        glDrawArrays(GL_TRIANGLES, 0, axis->getnumvertices());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE)) {
        glfwSetWindowShouldClose(window, true);
    }
    float cameraSpeed = 2.5 * camera->deltaTime;
    
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        camera->cameraPos += cameraSpeed * camera->cameraFront;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        camera->cameraPos -= cameraSpeed * camera->cameraFront;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        camera->cameraPos -= glm::normalize(glm::cross(camera->cameraFront, camera->cameraUp)) * cameraSpeed;
        
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera->cameraPos += glm::normalize(glm::cross(camera->cameraFront, camera->cameraUp)) * cameraSpeed;
    }
}

void mouse_callback(GLFWwindow * window, double xpos, double ypos)
{
    if (camera->firstMouse)
    {
        
        camera->lastX = xpos;
        camera->lastY = ypos;
        camera->firstMouse = false;
    }
    
    float xoffset = xpos - camera->lastX;
    float yoffset = camera->lastY - ypos; // reversed since y-coordinates go from bottom to top
    camera->lastX = xpos;
    camera->lastY = ypos;
    
    float sensitivity = 0.1f; // change this value to your liking
    xoffset *= sensitivity;
    yoffset *= sensitivity;
    
    camera->yaw += xoffset;
    camera->pitch += yoffset;
    
    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (camera->pitch > 89.0f)
    camera->pitch = 89.0f;
    if (camera->pitch < -89.0f)
    camera->pitch = -89.0f;
    
    glm::vec3 front;
    front.x = cos(glm::radians(camera->yaw)) * cos(glm::radians(camera->pitch));
    front.y = sin(glm::radians(camera->pitch));
    front.z = sin(glm::radians(camera->yaw)) * cos(glm::radians(camera->pitch));
    camera->cameraFront = glm::normalize(front);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow * window, double xoffset, double yoffset)
{
    if (camera->fov >= 1.0f && camera->fov <= 45.0f)
    camera->fov -= yoffset;
    if (camera->fov <= 1.0f)
    camera->fov = 1.0f;
    if (camera->fov >= 45.0f)
    camera->fov = 45.0f;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// void glfw_window_size_callback(GLFWwindow * wndow, int widthx, int heightx)
// {
//         window->width = widthx;
//         window->height = heightx;
//         printf("width %i height %i\n", widthx, heightx);
//         /* update any perspective matrices used here */
//     }
