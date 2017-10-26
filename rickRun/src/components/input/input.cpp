#include "input.hpp"
#include "../sound/sound.hpp"
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

        // platformWorld->applyImpulse(1, btVector3(0,1.0,1.0));
        // camera_resetPerspective();
    }
    else if (xoffset > jumpSensitivity && yoffset < -jumpSensitivity)
    {
        // platformWorld->applyImpulse(1, btVector3(0, -1.0, 1.0));
        // camera_resetPerspective();
    }
    else if (xoffset < -jumpSensitivity && yoffset > jumpSensitivity)
    {
        // platformWorld->applyImpulse(1, btVector3(0, 1.0, -1.0));
        // camera_resetPerspective();
    }
    else if (xoffset < -jumpSensitivity && yoffset < -jumpSensitivity)
    {
        // platformWorld->applyImpulse(1, btVector3(0, -1.0, -1.0));
        // camera_resetPerspective();
    }

    float sensitivity = 0.01f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    // yaw += xoffset;
    // pitch += yoffset;

    // if (pitch > 89.0f)
    // pitch = 89.0f;
    // if (pitch < -89.0f)
    // pitch = -89.0f;

    // int xmid = g_gl_width / 2;
    // int ymid = g_gl_height / 2;

    // glm::vec3 front;

    // front.z = (glm::radians(xpos - xmid));
    // front.y = (glm::radians(-ypos + ymid));
    // front.x = 100;
    // cameraFront = glm::normalize(front);

    // lastX = g_gl_width / 2.0;
    // lastY = g_gl_height / 2.0;
    // glfwSetCursorPos(g_window, lastX, lastY);
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
    // int count;
    // const float* axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &count);
    //
    // cout << axes[0] << endl;
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);

    float cameraSpeed = 2.5 * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        // sun.x = sun.x + 0.1f;
        cameraPos += cameraSpeed * cameraFront;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        cameraPos -= cameraSpeed * cameraFront;
        // sun.x = sun.x - 0.1f;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        // sun.z = sun.z + 0.1f;
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        // sun.z = sun.z - 0.1f;
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        // sun.y = sun.y + 0.1f;
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        // sun.y = sun.y - 0.1f;
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    }

    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
    {
        cout << "SUN.X: " << sun.x << endl;
        cout << "SUN.Y: " << sun.y << endl;
        cout << "SUN.Z: " << sun.z << endl;
        // cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    }

    // btVector3 totalForceOnRick = platformWorld->getVelocity(1);

    // cout << fps << "     - ";
    // cout << "forceX: " << totalForceOnRick.getX()
    // << "  forceY: " << totalForceOnRick.getY()
    // << "   forceZ: " << totalForceOnRick.getZ() << endl;
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
        platformWorld->applyImpulse(1, btVector3(jumpForwardForce * deltaTime, 0.0, 0.0));
    }
    else if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
    {
        if (!allowJump && allowDownJump) {
            allowDownJump = false;
            platformWorld->setVelocity(1, btVector3(0, 0, 0));
            platformWorld->applyImpulse(1, btVector3(jumpBackwardForce, jumpVerticalDownForce * 100, 0.0));
        }
    }
    else if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
    {
        platformWorld->applyImpulse(1, btVector3(0.0, 0.0, -jumpHorizontalForce * deltaTime));
    }
    else if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
    {
        platformWorld->applyImpulse(1, btVector3(0.0, 0.0, jumpHorizontalForce * deltaTime));
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        if (allowJump) {
            allowJump = false;
            allowDownJump = true;
            platformWorld->applyImpulse(1, btVector3(0.0, jumpVerticalUpForce, 0.0f));
        }
    }
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
    {
        // platformWorld->applyImpulse(1, btVector3(0.0, 1.0f, 0.0f));
        // platformWorld->translate(1, btVector3(0.0, 0.0, 0.0));
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
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
    {
    //   if(snd_01->get_source_state() != AL_PLAYING)  {
    //     snd_01->play();
    //   }
    background[0]->play();
    }
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
    {
    //   if(snd_02->get_source_state() != AL_PLAYING)  {
    //     snd_02->play();
    //   }
    background[1]->play();
    }
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
    {
        cout << "3 presionado" << endl;
        // background[0]->play();
        // background[1]->play();
        // core->initBackgroundMusic();
    }


}

void input_framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}
