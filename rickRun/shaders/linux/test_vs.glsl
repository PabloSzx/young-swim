#version 130

//layout(location = 0) in vec3 vertex_position;
//layout(location = 1) in vec3 vertex_normal;
//layout(location = 2) in vec2 texture_coord;

in vec3 vertex_position;
in vec3 vertex_normal;
in vec2 texture_coord;

uniform mat4 view, proj, matrix;

uniform vec3 sun;

out vec3 light;
out vec3 normal;
out vec2 st;

void main() {
	vec3 lp = vec3(sun.x, sun.y, sun.z);
	light = normalize(matrix*vec4(vertex_position, 1.0f) - vec4(lp, 1.0f)).xyz;
	//light = (vec4(sun.x, sun.y, sun.z, 0.0f)).xyz;

	st = texture_coord;
	//normal = vertex_normal;
	normal = (vec4(vertex_normal, 0.0)).xyz;
	gl_Position = proj * view * matrix * vec4 (vertex_position, 1.0);
}
