#version 330 core
layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aUV;

out vec2 UV;

void main() 
{
	UV = aUV;
	gl_Position = vec4(aPos, 0.0, 1.0);
}
