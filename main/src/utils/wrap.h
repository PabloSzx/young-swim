#ifndef _WRAP_H_
#define _WRAP_H_

float cam_speed = 3.0f; // 1 unit per second
float cam_pos[] = {0.0f, 0.0f, 5.0f}; // don't start at zero, or we will be too close
float cam_yaw_speed = 30.0f; // 10 degrees per second

float cam_yaw = 0.0f; // y-rotation in degrees

vec3 campos(0.0f, 0.0f, 5.0f);
mat4 T = translate (identity_mat4 (), vec3 (-cam_pos[0], -cam_pos[1], -cam_pos[2]));
mat4 R = rotate_y_deg (identity_mat4 (), -cam_yaw);
mat4 view_mat = R * T;

int view_mat_location;

void cameraSetup(GLuint shader_programme) {
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

	int view_mat_location = glGetUniformLocation (shader_programme, "view");
	glUseProgram (shader_programme);
	glUniformMatrix4fv (view_mat_location, 1, GL_FALSE, view_mat.m);
	int proj_mat_location = glGetUniformLocation (shader_programme, "proj");
	glUseProgram (shader_programme);
	glUniformMatrix4fv (proj_mat_location, 1, GL_FALSE, proj_mat);

}

void move(int matrix_location, glm::vec3 trans) {
	glm::mat4 mat = glm::mat4();

	mat = glm::translate(mat, trans);
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, &mat[0][0]);
}


#endif
