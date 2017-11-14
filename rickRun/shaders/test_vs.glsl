#version 130

// valores de entrada, cada thread recibe uno distinto.
in vec3 vertex_position;
in vec3 vertex_normal;
in vec2 texture_coord;

// variables comunes para todos los shaders
uniform mat4 view, proj, matrix;

// variables de salida, cada shader genera un valor distinto
out vec3 light;
out vec3 normal;
out vec2 st;
out vec3 normal_eye;

void main(){
	vec3 light_pos = vec3(0.5f, 1.0f, 0.5f);
    light = (view * vec4(light_pos, 0.0f)).xyz;
	st = texture_coord;
	normal = (view * matrix * vec4(vertex_normal, 0.0)).xyz;
	gl_Position = proj * view * matrix * vec4(vertex_position, 1.0);
	normal_eye = vec3(view * matrix * vec4(vertex_normal, 0.0));
}
