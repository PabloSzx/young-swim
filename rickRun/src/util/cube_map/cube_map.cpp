#include "cube_map.hpp"

GLuint make_big_cube()
{
    float points[] = {
        -20.0f, 20.0f, -20.0f,
        -20.0f, -20.0f, -20.0f,
        20.0f, -20.0f, -20.0f,
        20.0f, -20.0f, -20.0f,
        20.0f, 20.0f, -20.0f,
        -20.0f, 20.0f, -20.0f,

        -20.0f, -20.0f, 20.0f,
        -20.0f, -20.0f, -20.0f,
        -20.0f, 20.0f, -20.0f,
        -20.0f, 20.0f, -20.0f,
        -20.0f, 20.0f, 20.0f,
        -20.0f, -20.0f, 20.0f,

        20.0f, -20.0f, -20.0f,
        20.0f, -20.0f, 20.0f,
        20.0f, 20.0f, 20.0f,
        20.0f, 20.0f, 20.0f,
        20.0f, 20.0f, -20.0f,
        20.0f, -20.0f, -20.0f,

        -20.0f, -20.0f, 20.0f,
        -20.0f, 20.0f, 20.0f,
        20.0f, 20.0f, 20.0f,
        20.0f, 20.0f, 20.0f,
        20.0f, -20.0f, 20.0f,
        -20.0f, -20.0f, 20.0f,

        -20.0f, 20.0f, -20.0f,
        20.0f, 20.0f, -20.0f,
        20.0f, 20.0f, 20.0f,
        20.0f, 20.0f, 20.0f,
        -20.0f, 20.0f, 20.0f,
        -20.0f, 20.0f, -20.0f,

        -20.0f, -20.0f, -20.0f,
        -20.0f, -20.0f, 20.0f,
        20.0f, -20.0f, -20.0f,
        20.0f, -20.0f, -20.0f,
        -20.0f, -20.0f, 20.0f,
        20.0f, -20.0f, 20.0f};
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(
        GL_ARRAY_BUFFER, 3 * 36 * sizeof(GLfloat), &points, GL_STATIC_DRAW);

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    return vao;
}

bool load_cube_map_side(
    GLuint texture, GLenum side_target, const char *file_name)
{
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

    int x, y, n;
    int force_channels = 4;
    unsigned char *image_data = stbi_load(
        file_name, &x, &y, &n, force_channels);
    if (!image_data)
    {
        fprintf(stderr, "ERROR: could not load %s\n", file_name);
        return false;
    }
    // non-power-of-2 dimensions check
    if ((x & (x - 1)) != 0 || (y & (y - 1)) != 0)
    {
        fprintf(
            stderr, "WARNING: image %s is not power-of-2 dimensions\n", file_name);
    }

    // copy image data into 'target' side of cube map
    glTexImage2D(
        side_target,
        0,
        GL_RGBA,
        x,
        y,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        image_data);
    free(image_data);
    return true;
}

void create_cube_map(
    const char *front,
    const char *back,
    const char *top,
    const char *bottom,
    const char *left,
    const char *right,
    GLuint *tex_cube)
{
    // generate a cube-map texture to hold all the sides
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, tex_cube);

    // load each image and copy into a side of the cube-map texture
    assert(load_cube_map_side(*tex_cube, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, front));
    assert(load_cube_map_side(*tex_cube, GL_TEXTURE_CUBE_MAP_POSITIVE_Z, back));
    assert(load_cube_map_side(*tex_cube, GL_TEXTURE_CUBE_MAP_POSITIVE_Y, top));
    assert(load_cube_map_side(*tex_cube, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, bottom));
    assert(load_cube_map_side(*tex_cube, GL_TEXTURE_CUBE_MAP_NEGATIVE_X, left));
    assert(load_cube_map_side(*tex_cube, GL_TEXTURE_CUBE_MAP_POSITIVE_X, right));
    // format cube map texture
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

GLuint create_programme_from_files(
    const char *vert_file_name, const char *frag_file_name)
{
    GLuint vert, frag, programme;
    assert(shader_create_shader(vert_file_name, &vert, GL_VERTEX_SHADER));
    assert(shader_create_shader(frag_file_name, &frag, GL_FRAGMENT_SHADER));
    assert(shader_create_programme(vert, frag, &programme));
    return programme;
}

void initializeCube() {
    cube_vao = make_big_cube();
    create_cube_map(FRONT, BACK, TOP, BOTTOM, LEFT, RIGHT, &cube_map_texture);

    cube_sp = create_programme_from_files(
        CUBE_VERT_FILE, CUBE_FRAG_FILE);

    cube_V_location = glGetUniformLocation(cube_sp, "V");
    cube_P_location = glGetUniformLocation(cube_sp, "P");
    // glm::mat4 a = glm::rotate a
    // glm::mat4 R = glm::rotate(glm::mat4(), glm::radians(-yaw), glm::vec3(0.0, 1.0, 0.0));
    
    glUseProgram(cube_sp);
    // glUniformMatrix4fv(cube_V_location, 1, GL_FALSE, &view[0][0]);
    glm::mat4 R = glm::lookAt(glm::vec3(0.0f), cameraFront, glm::vec3(0.0f, 1.0f, 0.0f));
    glUniformMatrix4fv(cube_V_location, 1, GL_FALSE, &R[0][0]);

    glUniformMatrix4fv(cube_P_location, 1, GL_FALSE, &projection[0][0]);

}

void drawCube() {
    // glUseProgram(cube_sp);
    // glm::mat4 R = glm::rotate(glm::mat4(), glm::radians(-yaw), glm::vec3(0.0, 1.0, 0.0));
    // R = glm::rotate(R, glm::radians(pitch), glm::vec3(1.0, 0.0, 0.0));

    // glUniformMatrix4fv(cube_V_location, 1, GL_FALSE, &view[0][0]);
    // glm::mat4 R = glm::lookAt(glm::vec3(0.0f), cameraFront, glm::vec3(0.0f, 1.0f, 0.0f));
    // glUniformMatrix4fv(cube_V_location, 1, GL_FALSE, &R[0][0]);

    glDepthMask(GL_FALSE);
    glUseProgram(cube_sp);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cube_map_texture);
    glBindVertexArray(cube_vao);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glDepthMask(GL_TRUE);
}

void cameraCube() {
    // glUseProgram(cube_sp);
    // glm::mat4 R = glm::rotate(glm::mat4(), glm::radians(-yaw), glm::vec3(0.0, 1.0, 0.0));
    // R = glm::rotate(R, glm::radians(pitch), glm::vec3(1.0, 0.0, 0.0));
    glm::mat4 R = glm::lookAt(glm::vec3(0.0f), cameraFront, glm::vec3(0.0f, 1.0f, 0.0f));
    glUniformMatrix4fv(cube_V_location, 1, GL_FALSE, &R[0][0]);

    glUniformMatrix4fv(cube_V_location, 1, GL_FALSE, &projection[0][0]);

    // glUniformMatrix4fv(cube_P_location, 1, GL_FALSE, &projection[0][0]);
}
