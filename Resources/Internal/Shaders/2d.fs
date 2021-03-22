#version 430

in vec2 pass_tex_coord;
in vec4 pass_color;

out vec4 out_color;

layout(binding = 0) uniform sampler2D atlas;

void main()
{
	out_color = texture(atlas, pass_tex_coord) * pass_color;
}