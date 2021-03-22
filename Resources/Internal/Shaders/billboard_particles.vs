#version 430 core

layout(location = 0) in vec2 in_position;
layout(location = 1) in vec2 in_tex_coord;

out vec4 pass_color;
out vec2 pass_tex_coord;

layout(location = 0) uniform mat4 P_V_T; // In reference to this matrix, only the position and rotation of the entire system's transform are accounted for. Positions are scaled according to the transform, but the size of the particles are not.
layout(location = 1) uniform float inverse_apsect_ratio;
layout(location = 2) uniform vec3 relative_player_pos;
layout(location = 3) uniform float fade_start;
layout(location = 4) uniform float fade_end;

struct ParticleData
{
	vec4 positionAndScale;
	vec4 color;
	vec4 subTexCoords;
};

layout(std430, binding = 0) buffer instanceBuffer
{
    ParticleData instanceDataArr[];
};

void main()
{
	ParticleData data = instanceDataArr[gl_InstanceID];
	
	//gl_Position = P_V_T * vec4(data.positionAndScale.xyz + data.positionAndScale.w * vec3(in_position.x, in_position.y, 0.0), 1.0);
	gl_Position = P_V_T * vec4(data.positionAndScale.xyz, 1.0);
	gl_Position.xy += vec2(in_position.x * inverse_apsect_ratio, in_position.y) * data.positionAndScale.w;
	gl_Position /= gl_Position.w;
	
	pass_color = data.color * (1.0 - smoothstep(fade_start, fade_end, distance(relative_player_pos, data.positionAndScale.xyz)));
	pass_tex_coord = data.subTexCoords.xy + in_tex_coord * data.subTexCoords.zw;
}