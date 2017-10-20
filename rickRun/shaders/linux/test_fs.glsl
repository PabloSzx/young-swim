#version 130

in vec3 normal;
in vec2 st;
in vec3 light;
out vec4 frag_colour;
// out vec4 white;
// uniform vec3 color;
uniform sampler2D basic_texture;

void main() {
	
	vec4 texel = texture (basic_texture, st);
	// black = vec4(0.0, 0.0, 0.0, 1.0);
	// white = vec4(1.0, 1.0, 1.0, 1.0);
	// frag_colour = vec4 (color, 1.0);
	// frag_colour = texture (basic_texture, st);
	float i = dot(light, normal);
	frag_colour = texel * i;
}
