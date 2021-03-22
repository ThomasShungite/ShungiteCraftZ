#version 430

in vec2 pass_tex_coord;

out vec3 out_color;

layout(binding = 0) uniform sampler2D src;

layout(location = 0) uniform vec2 properties;
									 
const int indexMatrix[64] = int[](
    0, 32,  8, 40,  2, 34, 10, 42,   /* 8x8 Bayer ordered dithering  */
    48, 16, 56, 24, 50, 18, 58, 26,  /* pattern.  Each input pixel   */
    12, 44,  4, 36, 14, 46,  6, 38,  /* is scaled to the 0..63 range */
    60, 28, 52, 20, 62, 30, 54, 22,  /* before looking in this table */
    3, 35, 11, 43,  1, 33,  9, 41,   /* to determine the action.     */
    51, 19, 59, 27, 49, 17, 57, 25,
    15, 47,  7, 39, 13, 45,  5, 37,
    63, 31, 55, 23, 61, 29, 53, 21 );

void main()
{
	vec2 coord = pass_tex_coord * 2.0 - 1.0;
	float rf = sin(sqrt(dot(coord, coord))) * ((0.25 + properties.y * 0.125) * properties.x);
	float rf2_1 = rf * rf + 1.0;
	float e = 1.0 / (rf2_1 * rf2_1);
	e = pow(e, 2.2);
	vec3 color = texture(src, pass_tex_coord).xyz * e;
	float gbDamp = (1.0 - (1.0 - e) * properties.y);
	color.gb *= gbDamp;
	color += 1.0 / 255.0 * vec3((indexMatrix[int(mod(gl_FragCoord.x, 8)) + int(mod(gl_FragCoord.y, 8)) * 8]) / 64.0 - (1.0 / 128.0));
	out_color = color;
}