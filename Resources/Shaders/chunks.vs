#version 430

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec2 in_tex_coords;

out vec3 pass_position;
out vec2 pass_tex_coords;
out vec3 pass_normal;

layout(location = 0) uniform mat4 p_v_t;

void main()
{
	gl_Position = p_v_t * vec4(in_position, 1.0);

	pass_position = in_position;
	pass_tex_coords = in_tex_coords;
	pass_normal = in_normal;
}
