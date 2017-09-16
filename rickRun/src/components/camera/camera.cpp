#include "camera.h"


void viewMatrixLocation()
{
    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    view_mat_location = glGetUniformLocation(shader_programme, "view");
    glUseProgram(shader_programme);
    glUniformMatrix4fv(view_mat_location, 1, GL_FALSE, &view[0][0]);
}

void projMatrixLocation()
{
    projection = glm::perspective(glm::radians(fov), (float)g_gl_width / (float)g_gl_height, 0.1f, 100.0f);
    proj_mat_location = glGetUniformLocation(shader_programme, "proj");
    glUseProgram(shader_programme);
    glUniformMatrix4fv(proj_mat_location, 1, GL_FALSE, &projection[0][0]);
}

void viewMatrixPerspective()
{
    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    glUniformMatrix4fv(view_mat_location, 1, GL_FALSE, &view[0][0]);
}

void projectionMatrixPerspective() {
    projection = glm::perspective(glm::radians(fov), (float)g_gl_width / (float)g_gl_height, 0.1f, 100.0f);
    glUniformMatrix4fv(proj_mat_location, 1, GL_FALSE, &projection[0][0]);
}