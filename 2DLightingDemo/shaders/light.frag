#version 330 core
out vec4 fragCol;

uniform vec4 lightColor = vec4(1.0, 0.0, 0.0, 1.0);
uniform vec2 lightPos = vec2(550.0, 450.0);
uniform int light_radius;

void main() 
{
	float distance = length(lightPos - gl_FragCoord.xy);
	//float attenuation = light_radius * 1.0 / (1 + lin_const*distance + quad_const*pow(distance, 2));
	float attenuation = smoothstep(light_radius, 0, distance);
	fragCol = vec4(1.0, 1.0, 1.0, attenuation) * lightColor;
}