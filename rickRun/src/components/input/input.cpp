#include "input.hpp"
#include "../sound/sound.hpp"

using namespace std;
void input_setCallbacks()
{
    glfwSetFramebufferSizeCallback(g_window, input_framebuffer_size_callback);
    glfwSetCursorPosCallback(g_window, input_mouse_callback);
    glfwSetScrollCallback(g_window, input_scroll_callback);
}

void camera_resetPerspective()
{
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

    int jumpSensitivity = 5;
    if (xoffset > jumpSensitivity && yoffset > jumpSensitivity)
    {
    }
    else if (xoffset > jumpSensitivity && yoffset < -jumpSensitivity)
    {
    }
    else if (xoffset < -jumpSensitivity && yoffset > jumpSensitivity)
    {
    }
    else if (xoffset < -jumpSensitivity && yoffset < -jumpSensitivity)
    {
    }

    float sensitivity = 0.01f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

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
    if (glfwJoystickPresent(GLFW_JOYSTICK_1)) {
        joystickPresent = true;
        int count;
        int count2;
        const float *axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &count);

        const unsigned char *buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &count2);
        const char *name = glfwGetJoystickName(GLFW_JOYSTICK_1);

        if (jumpButton == -1)
        {
            for (int i = 0; i < count2; i++)
            {
                if (buttons[i] == GLFW_PRESS)
                {
                    jumpButton = i;
                    cout << "button " << i << " is jump" << endl;
                    break;
                }
            }
        } else if (resetButton == -1) {
            for (int i = 0; i < count2; i++)
            {
                if (buttons[i] == GLFW_PRESS && i != jumpButton)
                {
                    resetButton = i;
                    cout << "button " << i << " is reset" << endl;
                    break;
                }
            }
        }


        if (axes[1] < -0.2)
        {
            platformWorld->applyImpulse(1, btVector3(jumpForwardForce * deltaTime, 0.0, 0.0));
        }
        else if (axes[1] > 0.8)
        {
            if (!allowJump && allowDownJump)
            {
                allowDownJump = false;
                background[7]->stop();
                background[7]->play();
                platformWorld->setVelocity(1, btVector3(0, 0, 0));
                platformWorld->applyImpulse(1, btVector3(jumpBackwardForce, jumpVerticalDownForce * 100, 0.0));
            }
        }
        else if (axes[0] < -0.2)
        {
            platformWorld->applyImpulse(1, btVector3(0.0, 0.0, -jumpHorizontalForce * deltaTime));
        }
        else if (axes[0] > 0.2)
        {
            platformWorld->applyImpulse(1, btVector3(0.0, 0.0, jumpHorizontalForce * deltaTime));
        }
        cameraFront = glm::vec3(1.0f, -(axes[4] * 0.5) - 0.2f, axes[3] * 0.5);

        if (jumpButton != -1) {
            if (buttons[jumpButton] == GLFW_PRESS)
            {
                if (allowJump)
                {
                    allowJump = false;
                    allowDownJump = true;
                    background[6]->stop();
                    background[6]->play();
                    platformWorld->applyImpulse(1, btVector3(0.0, jumpVerticalUpForce, 0.0f));
                }
            }
            else if (resetButton != -1) {
                if (buttons[resetButton] == GLFW_PRESS) {
                    camera_reset();
                }
            }
        }
    }
    else {
        if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
        {
            platformWorld->applyImpulse(1, btVector3(jumpForwardForce * deltaTime, 0.0, 0.0));
        }
        else if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
        {
            if (!allowJump && allowDownJump)
            {
                allowDownJump = false;
                background[7]->stop();
                background[7]->play();
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
            if (allowJump)
            {
                allowJump = false;
                allowDownJump = true;
                background[6]->stop();
                background[6]->play();
                platformWorld->applyImpulse(1, btVector3(0.0, jumpVerticalUpForce, 0.0f));
            }
        }
    }

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
        cameraFree = true;
    }
    float cameraSpeed = 2.5 * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        cameraPos += cameraSpeed * cameraFront;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        cameraPos -= cameraSpeed * cameraFront;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        cameraPos = glm::vec3(cameraPos[0], cameraPos[1] + (10.0 * deltaTime), cameraPos[2]);
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        cameraPos = glm::vec3(cameraPos[0], cameraPos[1] - (10.0 * deltaTime), cameraPos[2]);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        cameraPos = glm::vec3(cameraPos[0], cameraPos[1], cameraPos[2] - (10.0 * deltaTime));
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        cameraPos = glm::vec3(cameraPos[0], cameraPos[1], cameraPos[2] + (10.0 * deltaTime));
    }
}

void input_framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}
