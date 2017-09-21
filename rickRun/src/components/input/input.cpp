#include "input.h"
// #include "../camera/camera.h"

using namespace std;
void input_setCallbacks()
{
    glfwSetFramebufferSizeCallback(g_window, input_framebuffer_size_callback);
    glfwSetCursorPosCallback(g_window, input_mouse_callback);
    glfwSetScrollCallback(g_window, input_scroll_callback);
}

void camera_resetPerspective() {
    cameraFront = glm::vec3(0.94f, -0.33f, -0.09f);
    cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    yaw = 0; // yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.

    pitch = -20.0f;
}

void input_mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    // cout << "xoffset: " << xoffset << "   -   " << "yoffset: " << yoffset << endl;
    // cout << "xpos: " << xpos << "   -   " << "ypos: " << ypos << endl;

    int jumpSensitivity = 5;
    if (xoffset > jumpSensitivity && yoffset > jumpSensitivity)
    {
        world->applyImpulse(1, btVector3(0,1.0,1.0));
        camera_resetPerspective();
    }
    else if (xoffset > jumpSensitivity && yoffset < -jumpSensitivity)
    {
        world->applyImpulse(1, btVector3(0, -1.0, 1.0));
        camera_resetPerspective();
    }
    else if (xoffset < -jumpSensitivity && yoffset > jumpSensitivity)
    {
        world->applyImpulse(1, btVector3(0, 1.0, -1.0));
        camera_resetPerspective();
    }
    else if (xoffset < -jumpSensitivity && yoffset < -jumpSensitivity)
    {
        world->applyImpulse(1, btVector3(0, -1.0, -1.0));
        camera_resetPerspective();
    }

    lastX = g_gl_width / 2.0;
    lastY = g_gl_height / 2.0;
    glfwSetCursorPos(g_window, lastX, lastY);

    float sensitivity = 0.01f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;
    
    yaw += xoffset;
    pitch += yoffset;
    
    if (pitch > 89.0f)
    pitch = 89.0f;
    if (pitch < -89.0f)
    pitch = -89.0f;
    
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}

void input_scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    if (fov >= 1.0f && fov <= 45.0f)
    fov -= yoffset;
    if (fov <= 1.0f)
    fov = 1.0f;
    if (fov >= 45.0f)
    fov = 45.0f;
}

void input_processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
    
    float cameraSpeed = 2.5 * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
        // world->applyImpulse(1, btVector3(0.0, 10.5, -10.25), btVector3(0.0, 1.0, 1.0));
        // vx -= 0.1;
        world->applyImpulse(1, btVector3(0.2, 0.0, 0.0));
    }
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
    {
        // world->applyImpulse(1, btVector3(0.0, -10.5, 10.25), btVector3(0.0, 1.0, 1.0));
        // vx += 0.1;
        world->applyImpulse(1, btVector3(-0.2, 0.0, 0.0));
    }
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
    {
        // world->applyImpulse(1, btVector3(-10.25, 0.0, 0.0), btVector3(1.0, 0.0, 0.0));
        // vz -= 0.1;
        world->applyImpulse(1, btVector3(0.0, 0.0, -0.2));
    }
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
    {
        // world->applyImpulse(1, btVector3(10.25, 0.0, 0.0), btVector3(1.0, 0.0, 0.0));
        // vz += 0.1;
        world->applyImpulse(1, btVector3(0.0, 0.0, 0.2));
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        if (allowJump) {
            allowJump = false;
            world->applyImpulse(1, btVector3(0.0, 3.0f, 0.0f));
        }
        // world->translate(1, btVector3(0.0, 0.0, 0.0));
    }
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
    {
        // world->applyImpulse(1, btVector3(0.0, 1.0f, 0.0f));
        // world->translate(1, btVector3(0.0, 0.0, 0.0));
        cout << endl << endl << endl;
        cout << "cameraUp" << cameraUp[0] << "/" << cameraUp[1] << "/" << cameraUp[2] << endl;
        cout << "cameraFront" << cameraFront[0] << "/" << cameraFront[1] << "/" << cameraFront[2] << endl;
        cout << "cameraPos" << cameraPos[0] << "/" << cameraPos[1] << "/" << cameraPos[2] << endl;
        cout << "fov " << fov << endl;
        cout << "lastx " << lastX << endl;
        cout << "lasty " << lastY << endl;
        cout << "yaw " << yaw << endl;
        cout << "pitch " << pitch << endl;
    }
}

void input_framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}
