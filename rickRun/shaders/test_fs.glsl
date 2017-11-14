#version 130
#define SPOTLIGHT_CUT 0.999f    // valor limite: es inverso al radio del cono de luz
#define SPECULAR_EXP 20
// in vec3 position_eye, normal_eye;
in vec3 view_dir_tan;
in vec3 light_dir_tan;
 
in vec2 st;
uniform mat4 view_mat;
 
uniform sampler2D texsamp_rgb;
uniform sampler2D texsamp_normal;
 
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
  // ambient intensity
  vec3 Ia = La * Ka;
    // vec3 Id = vec3(0.0f, 0.0f, 0.0f);
    // vec3 Is = vec3(0.0f, 0.0f, 0.0f);
  vec3 normal_tan = texture (texsamp_normal, st).rgb;
  vec3 texel      = texture (texsamp_rgb, st).rgb;
  normal_tan = normalize (normal_tan * 2.0 - 1.0);
 
    //vec3 light_position_eye = vec3(view_mat * vec4 (light_position_world, 1.0));
 
    // diffuse intensity
    // raise light position to eye space
    // vec3 distance_to_light_eye = light_position_eye - position_eye;
    // vec3 direction_to_light_eye = normalize (distance_to_light_eye);
    // float dot_prod = dot (direction_to_light_eye, normal_eye);
    // dot_prod = max (dot_prod, 0.0);
    vec3 direction_to_light_tan = normalize (-light_dir_tan);
  float dot_prod = dot (direction_to_light_tan, normal_tan);
  dot_prod = max (dot_prod, 0.0);
 
    vec3 Id = Ld.rgb * Kd.rgb * texel * dot_prod;
 
    //Id = Ld * Kd * texel * dot_prod; // final diffuse intensity

	// specular intensity (blinn)
    // vec3 surface_to_viewer_eye = normalize (-position_eye);
    // vec3 half_way_eye = normalize (surface_to_viewer_eye + direction_to_light_eye);
    // float dot_prod_specular = max (dot (half_way_eye, normal_eye), 0.0);
    // float specular_factor = pow (dot_prod_specular, specular_exponent);
    // Is = Ls * Ks * specular_factor; // final specular intensity
    vec3 reflection_tan = reflect (normalize (light_dir_tan), normal_tan);
  float dot_prod_specular = dot (reflection_tan, normalize (view_dir_tan));
  dot_prod_specular = max (dot_prod_specular, 0.0);
  float specular_factor = pow (dot_prod_specular, SPECULAR_EXP);
  //vec3 Is = vec3 (1.0, 1.0, 1.0) * vec3 (0.5, 0.5, 0.5) * specular_factor;
  vec3 Is = Ls.rgb * Ks.rgb * specular_factor;
 
  // final colour
  // vec4 texel = texture (basic_texture, st);
  //fragment_colour = 0.5 * vec4 (Is + Id + Ia, 1.0) + texel;
  // fragment_colour = vec4 (Is + Id*texel.xyz + Ia*texel.xyz, 1.0);
    fragment_colour.rgb = Is + Id + Ia;
    fragment_colour.a = 1.0;
 
}