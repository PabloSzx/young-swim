#include "camera.h"

void camera_viewMatrixLocation()
{
    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    view_mat_location = glGetUniformLocation(shader_programme, "view");
    glUseProgram(shader_programme);
    glUniformMatrix4fv(view_mat_location, 1, GL_FALSE, &view[0][0]);
}

void camera_projMatrixLocation()
{
    projection = glm::perspective(glm::radians(fov), (float)g_gl_width / (float)g_gl_height, 0.1f, 100.0f);
    proj_mat_location = glGetUniformLocation(shader_programme, "proj");
    glUseProgram(shader_programme);
    glUniformMatrix4fv(proj_mat_location, 1, GL_FALSE, &projection[0][0]);
}

void camera_viewMatrixPerspective(glm::vec3 charPos)
{
    view = glm::lookAt(cameraPos + charPos, cameraPos + charPos + cameraFront, cameraUp);
    glUniformMatrix4fv(view_mat_location, 1, GL_FALSE, &view[0][0]);
}

void camera_projectionMatrixPerspective()
{
    projection = glm::perspective(glm::radians(fov), (float)g_gl_width / (float)g_gl_height, 0.1f, 100.0f);
    glUniformMatrix4fv(proj_mat_location, 1, GL_FALSE, &projection[0][0]);
}