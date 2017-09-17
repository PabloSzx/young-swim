#version 410

// in vec3 normal;
// in vec2 st;
out vec4 frag_colour;
// out vec4 white;
uniform vec3 color;

void main() {
	frag_colour = vec4 (color, 1.0);
	// black = vec4(0.0, 0.0, 0.0, 1.0);
	// white = vec4(1.0, 1.0, 1.0, 1.0);
}
