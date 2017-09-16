#include "input.h"

void setCallbacks() {
    glfwSetFramebufferSizeCallback(g_window, framebuffer_size_callback);
    glfwSetCursorPosCallback(g_window, mouse_callback);
    glfwSetScrollCallback(g_window, scroll_callback);
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;
    
    float sensitivity = 0.1f;
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

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    if (fov >= 1.0f && fov <= 45.0f)
    fov -= yoffset;
    if (fov <= 1.0f)
    fov = 1.0f;
    if (fov >= 45.0f)
    fov = 45.0f;
}

void processInput(GLFWwindow *window)
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
        world->applyImpulse(1, btVector3(0.0, 0.0, -0.1));
    }
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
    {
        // world->applyImpulse(1, btVector3(0.0, -10.5, 10.25), btVector3(0.0, 1.0, 1.0));
        // vx += 0.1;
        world->applyImpulse(1, btVector3(0.0, 0.0, 0.1));
    }
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
    {
        // world->applyImpulse(1, btVector3(-10.25, 0.0, 0.0), btVector3(1.0, 0.0, 0.0));
        // vz -= 0.1;
        world->applyImpulse(1, btVector3(-0.1, 0.0, 0.0));
    }
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
    {
        // world->applyImpulse(1, btVector3(10.25, 0.0, 0.0), btVector3(1.0, 0.0, 0.0));
        // vz += 0.1;
        world->applyImpulse(1, btVector3(0.1, 0.0, 0.0));
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        world->applyImpulse(1, btVector3(0.0, 1.0f, 0.0f));
        // world->translate(1, btVector3(0.0, 0.0, 0.0));
    }
}

void framebuffer_size_callback(GLFWwindow * window, int width, int height)
{
    glViewport(0, 0, width, height);
}
