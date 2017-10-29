#version 130
#define SPOTLIGHT_CUT 0.999f    // valor limite: es inverso al radio del cono de luz

in vec3 position_eye, normal_eye;
in vec2 st;
uniform sampler2D basic_texture;
uniform mat4 view_mat;

// fixed point light properties and spotlight direction
vec3 light_position_world  = vec3 (0.0, 0.0, 5.0);

vec3 Ls = vec3 (1.0, 1.0, 1.0); // white specular colour
vec3 Ld = vec3 (1.0, 1.0, 1.0); // dull white diffuse light colour
vec3 La = vec3 (0.3, 0.3, 0.3); // grey ambient colour
  
// surface reflectance
vec3 Ks = vec3 (1.0, 1.0, 1.0); // fully reflect specular light
vec3 Kd = vec3 (1.0, 1.0, 1.0); // diffuse surface reflectance
vec3 Ka = vec3 (1.0, 1.0, 1.0); // fully reflect ambient light
float specular_exponent = 300.0; // specular 'power'

out vec4 fragment_colour; // final colour of surface
void main () {
	vec4 texel = texture (basic_texture, st);
	//fragment_colour = 0.5 * vec4 (Is + Id + Ia, 1.0) + texel;
	fragment_colour = texel;
}
