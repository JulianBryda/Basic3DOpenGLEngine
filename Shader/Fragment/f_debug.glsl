#version 460 core

out vec4 FragColor;

uniform vec3 viewpos;

in vec3 fragPos;
in vec3 objPos;
in vec3 fragNorm;

void main()
{
	vec3 norm = normalize(fragNorm);

	vec3 viewDir = normalize(viewpos - fragPos);
	float dotProd = dot(norm, viewDir);
	float diff = max(dotProd, 0.0);
	
	FragColor = vec4(diff * vec3(1.0), 1.0);
} 