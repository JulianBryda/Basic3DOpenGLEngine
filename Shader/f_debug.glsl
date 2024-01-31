#version 460 core

out vec4 FragColor;

uniform vec4 color;
uniform vec3 camPos;

in vec3 fragPos;
in vec3 fragNorm;

void main()
{
	vec3 norm = normalize(fragNorm);

	vec3 lightDir = normalize(camPos - fragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	
	FragColor = vec4(diff * color.rgb, color.a);
} 