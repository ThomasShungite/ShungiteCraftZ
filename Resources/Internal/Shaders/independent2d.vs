#version 430

layout(location = 0) in vec2 in_position;
layout(location = 1) in vec2 in_tex_coord;

out vec2 pass_tex_coord;

layout(location = 0) uniform mat3 transformation;

void main()
{
	gl_Position = vec4((transformation * vec3(in_position, 1.0)).xy, 0.0, 1.0);
	pass_tex_coord = in_tex_coord;
}