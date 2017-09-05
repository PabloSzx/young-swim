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
#include <iostream>
#include <assert.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <string>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "constants.h"
#include "utils/maths_funcs.cpp"
#include "utils/gl_utils.cpp"
#include "tools.h"
#include "model.h"
#include "utils/wrap.h"

// keep track of window size for things like the viewport and the mouse cursor
int g_gl_width = 1280;
int g_gl_height = 690;
GLFWwindow* g_window = NULL;

using namespace std;
int main(){
	restart_gl_log ();
	start_gl ();
	glEnable (GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc (GL_LESS); // depth-testing interprets a smaller value as "closer"
	glEnable (GL_CULL_FACE); // cull face
	glCullFace (GL_BACK); // cull back face
	glFrontFace (GL_CCW); // set counter-clock-wise vertex order to mean the front
	glClearColor (0.2, 0.2, 0.2, 1.0); // grey background to help spot mistakes
	glViewport (0, 0, g_gl_width, g_gl_height);

	/* objeto enemigo */
	model *rick = new model((char*)RICK);

	model *suelo = new model((char*)SUELO);

	/*-------------------------------CREATE SHADERS-------------------------------*/
	GLuint shader_programme = create_programme_from_files (
		VERTEX_SHADER_FILE, FRAGMENT_SHADER_FILE
	);

	glm::mat4 mat = glm::mat4();

	int matrix_location = glGetUniformLocation (shader_programme, "matrix");
	glUseProgram (shader_programme);
	glUniformMatrix4fv (matrix_location, 1, GL_FALSE,  &mat[0][0]);

	cameraSetup(shader_programme);
	float dx = 0.0f;
	float dy = 0.0f;

	float limiteSuelo = 20.0f;

	while (!glfwWindowShouldClose (g_window)) {
		static double previous_seconds = glfwGetTime ();
		double current_seconds = glfwGetTime ();
		double elapsed_seconds = current_seconds - previous_seconds;
		previous_seconds = current_seconds;

		// control keys
		bool cam_moved = gameplay(cam_speed, elapsed_seconds, cam_pos, &cam_yaw, cam_yaw_speed, &dx, &dy);
		// update view matrix
		if (cam_moved){
			mat4 T = translate (identity_mat4 (), vec3 (-cam_pos[0], -cam_pos[1], -cam_pos[2])); // cam translation
			mat4 R = rotate_y_deg(identity_mat4 (), -cam_yaw); //
			mat4 view_mat = R * T;
			glUniformMatrix4fv(view_mat_location, 1, GL_FALSE, view_mat.m);
		}

		_update_fps_counter (g_window);
		// wipe the drawing surface clear
		glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport (0, 0, g_gl_width, g_gl_height);

		glUseProgram (shader_programme);

		move(matrix_location, glm::vec3(dx, dy, 0.0));

		glBindVertexArray(rick->getvao());
		glDrawArrays(GL_TRIANGLES,0,rick->getnumvertices());

		glBindVertexArray(suelo->getvao());
		for (float i = -limiteSuelo; i <= limiteSuelo; i++) {
			for (float j = -limiteSuelo; j <= limiteSuelo; j++) {
				move(matrix_location, glm::vec3(i, 1.0f, j));
				glDrawArrays(GL_TRIANGLES,0,suelo->getnumvertices());
			}
		}

		// update other events like input handling
		glfwPollEvents();

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
