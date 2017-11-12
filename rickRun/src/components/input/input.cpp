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
    if (glfwJoystickPresent(GLFW_JOYSTICK_1)) {
        int count;
        int count2;
        const float *axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &count);

        const unsigned char *buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &count2);
        // cout << "count: " << count << endl;
        // cout << "axes: " << axes << endl;
        const char *name = glfwGetJoystickName(GLFW_JOYSTICK_1);
        // cout << "name: " << name << endl;
        // cout << "x: " << axes[0] << endl;
        // cout << "y: " << axes[1] << endl;
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
                // cout << "button " << i << ": " << buttons[i] << endl;
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
                // cout << "button " << i << ": " << buttons[i] << endl;
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
            else if (resetButton != -1 && restart == false && timer->getNow() >= 3.0)
            {
                if (buttons[resetButton] == GLFW_PRESS)
                {
                    restart = true;
                    background[8]->stop();
                    background[8]->play();
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

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        debug = !debug;
        platformWorld->m_pDebugDrawer->ToggleDebugFlag(btIDebugDraw::DBG_DrawWireframe);
    }

    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        debug = !debug;
        platformWorld->m_pDebugDrawer->ToggleDebugFlag(btIDebugDraw::DBG_DrawAabb);
    }
    // float cameraSpeed = 2.5 * deltaTime;
    // if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    // {
    //     // sun.x = sun.x + 0.1f;
    //     cameraPos += cameraSpeed * cameraFront;
    // }
    // if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    // {
    //     cameraPos -= cameraSpeed * cameraFront;
    //     // sun.x = sun.x - 0.1f;
    // }
    // if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    // {
    //     // sun.z = sun.z + 0.1f;
    //     cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    // }
    // if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    // {
    //     // sun.z = sun.z - 0.1f;
    //     cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    // }
    // if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    // {
    //     // sun.y = sun.y + 0.1f;
    //     cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    // }
    // if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    // {
    //     // sun.y = sun.y - 0.1f;
    //     cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    // }

    // if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
    // {
    //     cout << "SUN.X: " << sun.x << endl;
    //     cout << "SUN.Y: " << sun.y << endl;
    //     cout << "SUN.Z: " << sun.z << endl;
    //     // cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    // }

}

void input_framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}
