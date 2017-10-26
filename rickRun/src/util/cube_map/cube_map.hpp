#ifndef CUBE_MAP
#define CUBE_MAP

#include <GL/glew.h>    // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "../../data/constants.hpp"
#include "../stb/stb_image.hpp"
#include "../shader/shader.hpp"

extern glm::mat4 view;
extern glm::mat4 projection;
extern glm::mat4 view;
extern glm::mat4 projection;

extern int cube_V_location;
extern int cube_P_location;

extern GLuint cube_sp;
extern GLuint cube_vao;
extern GLuint cube_map_texture;

extern float yaw;
extern float pitch;

extern glm::vec3 cameraFront;

void initializeCube();

void drawCube();
void cameraCube();

GLuint create_programme_from_files(const char *vert_file_name,
                                   const char *frag_file_name);
void create_cube_map(const char *front,
                     const char *back,
                     const char *top,
                     const char *bottom,
                     const char *left,
                     const char *right,
                     GLuint *tex_cube);
bool load_cube_map_side(
    GLuint texture, GLenum side_target, const char *file_name);

GLuint make_big_cube();

#endif
