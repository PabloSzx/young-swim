#version 130

in vec3 normal;
in vec2 st;
in vec3 light;
in vec3 normal_eye;
out vec4 frag_colour;
uniform sampler2D texsamp_rgb;
uniform sampler2D texsamp_normal;

#define SPECULAR_EXP 100
vec4 la = vec4(0.05, 0.05, 0.05, 1.0);
vec4 ld = vec4(1.0, 1.0, 1.0, 1.0);
vec4 ls = vec4(0.5, 0.5, 0.5, 1.0);

// material objeto
vec4 ka = vec4(1.0, 1.0, 1.0, 1.0);
vec4 kd = vec4(1.0, 1.0, 1.0, 1.0);
vec4 ks = vec4(0.7, 0.7, 0.7, 1.0);

void main() {
	vec4 texel = texture (texsamp_rgb, st);
    vec3 Ia = la.xyz * ka.xyz * texel.xyz;
	vec3 Id = dot(light, normal)*kd.xyz*ld.xyz*texel.xyz;

	vec3 r = -(light - (dot(light,normal)*2*normal));

	float dot_prod_specular = dot (normalize(r) , normalize (normal_eye));
	dot_prod_specular = max (dot_prod_specular, 0.0);
	float specular_factor = pow (dot_prod_specular, SPECULAR_EXP);

	vec3 Is = ls.xyz * ks.xyz * specular_factor; 

	frag_colour = vec4(Ia+Id+Is,1.0);
}
 

