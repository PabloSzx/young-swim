/******************************************************************************\
| OpenGL 4 Example Code.                                                       |
| Accompanies written series "Anton's OpenGL 4 Tutorials"                      |
| Email: anton at antongerdelan dot net                                        |
| First version 27 Jan 2014                                                    |
| Copyright Dr Anton Gerdelan, Trinity College Dublin, Ireland.                |
| See individual libraries for separate legal notices                          |
|******************************************************************************|
| This demo uses the Assimp library to load a mesh from a file, and supports   |
| many formats. The library is VERY big and complex. It's much easier to write |
| a simple Wavefront .obj loader. I have code for this in other demos. However,|
| Assimp will load animated meshes, which will we need to use later, so this   |
| demo is a starting point before doing skinning animation                     |
\******************************************************************************/
#include <assimp/cimport.h> // C importer
#include <assimp/scene.h> // collects data
#include <assimp/postprocess.h> // various extra operations
#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include "constants.h"
#include "maths_funcs.h"
#include "gl_utils.h"
#include "tools.h"
#include "model.h"

// keep track of window size for things like the viewport and the mouse cursor
int g_gl_width = 1280;
int g_gl_height = 690;
GLFWwindow* g_window = NULL;

int main(){
  restart_gl_log ();
  start_gl ();
  glEnable (GL_DEPTH_TEST); // enable depth-testing
  glDepthFunc (GL_LESS); // depth-testing interprets a smaller value as "closer"
  glEnable (GL_CULL_FACE); // cull face
  glCullFace (GL_BACK); // cull back face
  glFrontFace (GL_CCW); // set counter-clock-wise vertex order to mean the front
  glClearColor (0.2, 0.2, 0.2, 1.0); // grey background to help spot mistakes
  //glViewport (0, 0, g_gl_width, g_gl_height);

  /* objeto model */
  model *rick = new model((char*)"mallas/rick.obj");
  model *plataforma = new model((char*)"mallas/plataforma.obj");
  model *suelo = new model((char*)"mallas/suelo.obj");


  /*-------------------------------CREATE SHADERS-------------------------------*/
  GLuint shader_programme = create_programme_from_files (
    VERTEX_SHADER_FILE, FRAGMENT_SHADER_FILE
  );

  #define ONE_DEG_IN_RAD (2.0 * M_PI) / 360.0 // 0.017444444
  // input variables
  float near = 0.1f; // clipping plane
  float far = 100.0f; // clipping plane
  float fov = 67.0f * ONE_DEG_IN_RAD; // convert 67 degrees to radians
  float aspect = (float)g_gl_width / (float)g_gl_height; // aspect ratio
  // matrix components
  float range = tan (fov * 0.5f) * near;
  float Sx = (2.0f * near) / (range * aspect + range * aspect);
  float Sy = near / range;
  float Sz = -(far + near) / (far - near);
  float Pz = -(2.0f * far * near) / (far - near);
  GLfloat proj_mat[] = {
    Sx, 0.0f, 0.0f, 0.0f,
    0.0f, Sy, 0.0f, 0.0f,
    0.0f, 0.0f, Sz, -1.0f,
    0.0f, 0.0f, Pz, 0.0f
  };


  float cam_speed = 3.0f; // 1 unit per second
  float cam_yaw_speed = 90.0f; // 10 degrees per second
  float cam_pos[] = {0.0f, 5.0f, 1.0f}; // don't start at zero, or we will be too close
  vec3 campos(0.0f, 0.0f, 1.0f);

  float cam_yaw = 0.0f; // y-rotation in degrees
  float cam_xaw = 0.0f; // x-rotation in degrees

  mat4 T = translate (identity_mat4 (), vec3 (-cam_pos[0], -cam_pos[1], -cam_pos[2]));
  mat4 R = rotate_y_deg (identity_mat4 (), -cam_yaw);
  mat4 view_mat = R * T;

  int view_mat_location = glGetUniformLocation (shader_programme, "view");
  glUseProgram (shader_programme);
  glUniformMatrix4fv (view_mat_location, 1, GL_FALSE, view_mat.m);
  int proj_mat_location = glGetUniformLocation (shader_programme, "proj");
  glUseProgram (shader_programme);
  glUniformMatrix4fv (proj_mat_location, 1, GL_FALSE, proj_mat);

  glfwSetInputMode(g_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


  int matrix_location = glGetUniformLocation (shader_programme, "matrix");

  float dx = 0.0f;
  float dy = 0.0f;

  float limiteSuelo = 20.0f;


  while (!glfwWindowShouldClose (g_window)) {
    static double previous_seconds = glfwGetTime ();
    double current_seconds = glfwGetTime ();
    double elapsed_seconds = current_seconds - previous_seconds;
    previous_seconds = current_seconds;

    _update_fps_counter (g_window);
    // wipe the drawing surface clear
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram (shader_programme);
    //glBindVertexArray (monkey_vao);
    //glDrawArrays (GL_TRIANGLES, 0, monkey_point_count);
    glm::mat4 mat = glm::mat4();

    mat = glm::translate(mat, glm::vec3(dx, dy, 0.0));

    mat = glm::rotate(mat, (90.0f * 3.1415f) / 180.0f, glm::vec3(0.0f, 1.0f, 0.0f));

    glUniformMatrix4fv(matrix_location, 1, GL_FALSE, &mat[0][0]);
    //
    glBindVertexArray(rick->getvao());
    glDrawArrays(GL_TRIANGLES,0,rick->getnumvertices());
    //
    //
    mat = glm::mat4();

    mat = glm::translate(mat, glm::vec3(dx + 4.0f, dy + 1.3f, 1.5f));

    glUniformMatrix4fv(matrix_location, 1, GL_FALSE, &mat[0][0]);

    // move(matrix_location, glm::vec3(dx + 4.0f, dy + 1.3f, 1.5f));
    glBindVertexArray(plataforma->getvao());
    glDrawArrays(GL_TRIANGLES,0,plataforma->getnumvertices());



    glBindVertexArray(suelo->getvao());
    for (float i = -limiteSuelo; i <= limiteSuelo; i+=0.8f) {
      for (float j = -limiteSuelo; j <= limiteSuelo; j+=0.8f) {
        mat = glm::mat4();

        mat = glm::translate(mat, glm::vec3(i, -1.0f,j));

        glUniformMatrix4fv(matrix_location, 1, GL_FALSE, &mat[0][0]);

        // move(matrix_location, glm::vec3(i, -1.0f, j));
        glDrawArrays(GL_TRIANGLES,0,suelo->getnumvertices());
      }
    }


    // update other events like input handling
    glfwPollEvents();

    // control keys
    bool cam_moved = gameplay(cam_speed, elapsed_seconds, cam_pos, &cam_yaw, cam_yaw_speed, &cam_xaw, &dx, &dy);
    // update view matrix
    if (cam_moved){
			mat4 T = translate (identity_mat4 (), vec3 (-cam_pos[0], -cam_pos[1], -cam_pos[2])); // cam translation
			mat4 YR = rotate_y_deg(identity_mat4 (), -cam_xaw); //
			mat4 XR = rotate_x_deg(identity_mat4 (), -cam_yaw); //
			mat4 view_mat = (YR * XR) * T;
			glUniformMatrix4fv(view_mat_location, 1, GL_FALSE, view_mat.m);
		}
    if (GLFW_PRESS == glfwGetKey (g_window, GLFW_KEY_ESCAPE)) {
      glfwSetWindowShouldClose (g_window, 1);
    }
    // put the stuff we've been drawing onto the display
    glfwSwapBuffers (g_window);
  }

  // close GL context and any other GLFW resources
  glfwTerminate();
  return 0;
}
