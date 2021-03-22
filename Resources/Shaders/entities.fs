#version 430 core

in vec2 pass_tex_coords;
in vec3 pass_normal;

out vec3 out_color;

layout(location = 2) uniform vec3 directional_light_direction;
layout(location = 3) uniform vec3 directional_light_color;

layout(binding = 0) uniform sampler2D textureArray;

void main()
{	
	out_color = texture(textureArray, pass_tex_coords).rgb;
	out_color = vec3(1.0, 0.0, 0.0);
}
