#version 430 core

in vec2 pass_tex_coords;

layout(location = 0) out vec3 out_refraction_color;

layout(binding = 0) uniform sampler2D normal_and_ior_map;
layout(binding = 1) uniform sampler2D water_fog_map;
layout(binding = 2) uniform sampler2D frame_map;

void main()
{
	vec4 normalAndIor = texture(normal_and_ior_map, pass_tex_coords);
	vec3 normal = normalize(normalAndIor.xyz * 2.0 - vec3(1.0));
	float ior = normalAndIor.w * 2.0 + 1.0;
	
	vec4 fogInfo = texture(water_fog_map, pass_tex_coords);
	vec3 fogColor = fogInfo.xyz;
	float fogDensity = fogInfo.w;
	
	// TODO: Find a physical basis for the dampening factor
	float waterDistance = log2(fogDensity) / -0.15;
	vec2 refractOffset = refract(vec3(0.0, -1.0, 0.0), normal, 1.0 / ior).xz * waterDistance * 0.1;
	
	out_refraction_color = mix(fogColor, texture(frame_map, pass_tex_coords + refractOffset).rgb, fogDensity);
}