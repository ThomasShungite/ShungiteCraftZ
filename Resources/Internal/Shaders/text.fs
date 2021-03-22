#version 430

in vec2 pass_tex_coord;
in vec4 pass_color;

out vec4 out_color;

layout(binding = 0) uniform sampler2D atlas;

layout(location = 0) uniform float width_dist = 0.4;
layout(location = 1) uniform float edge_dist = 0.2;
layout(location = 2) uniform float border_width_dist = 0.5;
layout(location = 3) uniform float border_edge_dist = 0.1;
layout(location = 4) uniform vec4 border_color;

void main()
{
	float distance = 1.0 - texture(atlas, pass_tex_coord).a;
	float alpha = 1.0 - smoothstep(width_dist, width_dist + edge_dist, distance);
	float borderAlpha = 1.0 - smoothstep(border_width_dist, border_width_dist + border_edge_dist, distance);
	float overallAlpha = alpha + (1.0 - alpha) * borderAlpha;
	vec4 overallColor = mix(border_color, pass_color, alpha / overallAlpha);
	
	out_color = vec4(overallColor.rgb, overallColor.a * overallAlpha);
}