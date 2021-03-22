#version 430 core

in vec2 pass_tex_coords;

layout(location = 0) out vec3 out_reflection_color;

layout(binding = 0) uniform sampler2D normal_and_ior_map;
layout(binding = 1) uniform sampler2D out_ray_map;
layout(binding = 2) uniform samplerCube skybox;
layout(binding = 3) uniform sampler2D frame_map;
layout(binding = 4) uniform sampler2D depth_map;

void main()
{
	vec3 normal = normalize(texture(normal_and_ior_map, pass_tex_coords).xyz * 2.0 - vec3(1.0));
	vec3 outRay = normalize(texture(out_ray_map, pass_tex_coords).xyz * 2.0 - vec3(1.0));

	out_reflection_color = texture(skybox, reflect(-outRay, vec3(0.0, 1.0, 0.0))).rgb;
}