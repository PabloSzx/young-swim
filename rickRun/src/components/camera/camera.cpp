#include "camera.hpp"

void camera_viewMatrixLocation()
{
    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    view_mat_location = glGetUniformLocation(shader_programme, "view");
    glUseProgram(shader_programme);
    glUniformMatrix4fv(view_mat_location, 1, GL_FALSE, &view[0][0]);
}

void camera_viewMatrixLocation(GLuint shaderprog)
{
    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    view_mat_location = glGetUniformLocation(shaderprog, "view");
    glUseProgram(shaderprog);
    glUniformMatrix4fv(view_mat_location, 1, GL_FALSE, &view[0][0]);
}

void camera_projMatrixLocation()
{
    projection = glm::perspective(glm::radians(fov), (float)g_gl_width / (float)g_gl_height, 0.1f, 100.0f);
    proj_mat_location = glGetUniformLocation(shader_programme, "proj");
    glUseProgram(shader_programme);
    glUniformMatrix4fv(proj_mat_location, 1, GL_FALSE, &projection[0][0]);
}

void camera_projMatrixLocation(GLuint shaderprog)
{
    projection = glm::perspective(glm::radians(fov), (float)g_gl_width / (float)g_gl_height, 0.1f, 100.0f);
    proj_mat_location = glGetUniformLocation(shaderprog, "proj");
    glUseProgram(shaderprog);
    glUniformMatrix4fv(proj_mat_location, 1, GL_FALSE, &projection[0][0]);
}

void camera_viewMatrixPerspective(glm::vec3 charPos)
{
    if (!cameraFree) {
        view = glm::lookAt(cameraPos + charPos, cameraPos + charPos + cameraFront, cameraUp);
    } else {
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    }
    glUniformMatrix4fv(view_mat_location, 1, GL_FALSE, &view[0][0]);
}

void camera_viewMatrixPerspective()
{
    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    glUniformMatrix4fv(view_mat_location, 1, GL_FALSE, &view[0][0]);
}

void camera_projectionMatrixPerspective()
{
    projection = glm::perspective(glm::radians(fov), (float)g_gl_width / (float)g_gl_height, 0.1f, 100.0f);
    glUniformMatrix4fv(proj_mat_location, 1, GL_FALSE, &projection[0][0]);
}

void camera_reset() {
    cameraPos = glm::vec3(-14.4f, 2.52f, -4.3f);
    cameraFront = glm::vec3(1.0f, 0.0f, 0.0f);
    // cameraFrontInit = glm::vec3(1.0f, 0.0f, 0.0f);
    cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
}

void camera_viewProjUpdate() {
    double xpos, ypos;
    glfwGetCursorPos(g_window, &xpos, &ypos);
    if (joystickPresent) {
        if (cameraFree) {
            glm::vec3 front;

            front.z = (glm::radians(xpos));
            front.y = (glm::radians(-ypos));
            front.x = 100;
            cameraFront = glm::normalize(front);
        }
    } else {
        glfwSetCursorPos(g_window, xpos + deltaTime * 0.5 * ((g_gl_width / 2.0) - xpos), ypos + deltaTime * 0.5 * ((g_gl_height / 2.0 + 1800.0) - ypos));

        glfwGetCursorPos(g_window, &xpos, &ypos);

        int xmid = g_gl_width / 2;
        int ymid = g_gl_height / 2;

        glm::vec3 front;

        front.z = (glm::radians(xpos - xmid));
        front.y = (glm::radians(-ypos + ymid));
        front.x = 100;
        cameraFront = glm::normalize(front);
    }
    // if (joystickPresent && !cameraFree) {
    //     glfwSetCursorPos(g_window, xpos + deltaTime * 0.5 * ((g_gl_width / 2.0) - xpos), ypos + deltaTime * 0.5 * ((g_gl_height / 2.0 + 1800.0) - ypos));

    //     glfwGetCursorPos(g_window, &xpos, &ypos);

    //     int xmid = g_gl_width / 2;
    //     int ymid = g_gl_height / 2;

    //     glm::vec3 front;

    //     front.z = (glm::radians(xpos - xmid));
    //     front.y = (glm::radians(-ypos + ymid));
    //     front.x = 100;
    //     cameraFront = glm::normalize(front);
    // } else if (!joystickPresent && cameraFree) {
    //     glm::vec3 front;

    //     front.z = (glm::radians(xpos));
    //     front.y = (glm::radians(-ypos));
    //     front.x = 100;
    //     cameraFront = glm::normalize(front);
    // }
        camera_viewMatrixLocation();
        camera_projMatrixLocation();
        camera_projectionMatrixPerspective();
        camera_viewMatrixPerspective(glm::vec3(core->getRickPos().getX(), core->getRickPos().getY() + 2.0, core->getRickPos().getZ() + 4.5));

        camera_viewMatrixLocation(shader_programme_cube);
        camera_projMatrixLocation(shader_programme_cube);
        camera_projectionMatrixPerspective();
        camera_viewMatrixPerspective(glm::vec3(core->getRickPos().getX(), core->getRickPos().getY() + 2.0, core->getRickPos().getZ() + 4.5));
};

// void camera_resetPerspective() {
//     cameraFront = glm::vec3(0.94f, -0.33f, -0.09f);
//     cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

//     yaw = 0; // yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.

//     pitch = -20.0f;
// }