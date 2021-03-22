#version 430

in vec4 pass_color;
in vec2 pass_tex_coord;

out vec3 out_color;

layout(binding = 0) uniform sampler2D textureSampler;

void main()
{
	vec4 texColor = texture(textureSampler, pass_tex_coord);
	out_color = texColor.xyz * texColor.a * pass_color.xyz * pass_color.a;
}