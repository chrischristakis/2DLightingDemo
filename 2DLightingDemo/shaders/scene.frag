#version 330 core
out vec4 fragCol;
in vec2 UV;

uniform sampler2D stone_tex, light_tex;

void main()
{
	fragCol = texture(stone_tex, UV) * texture(light_tex, UV);
}