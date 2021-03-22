#version 430

in vec2 pass_tex_coord;
in vec4 pass_color;

out vec4 out_color;

layout(binding = 0) uniform sampler2D sprite;

layout(location = 1) uniform vec4 color;

void main()
{
	out_color = texture(sprite, pass_tex_coord) * color;
}