#version 130
//layout (location = 0) in vec3 vertex_position;
//layout (location = 1) in vec3 vertex_normal;
in vec3 vertex_position;
in vec3 vertex_normal;
in vec2 texture_coord;
uniform mat4 proj, view, matrix;
out vec3 position_eye, normal_eye;
out vec2 st;

void main () {
	position_eye = vec3 (view* matrix* vec4 (vertex_position, 1.0));
	normal_eye = vec3 (view* matrix* vec4 (vertex_normal, 0.0));
	st = texture_coord;
	gl_Position = proj * vec4 (position_eye, 1.0);
}
