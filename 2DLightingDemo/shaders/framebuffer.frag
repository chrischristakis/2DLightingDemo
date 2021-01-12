#version 330 core
out vec4 fragCol;
in vec2 UV;

uniform sampler2D tex;

void main() 
{
	fragCol = vec4(texture(tex, UV).rgb, 1.0);
}
