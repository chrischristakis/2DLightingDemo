#version 330 core
out vec4 fragCol;
in vec2 UV;

uniform sampler2D tex;

void main()
{
	fragCol = texture(tex, UV);
}