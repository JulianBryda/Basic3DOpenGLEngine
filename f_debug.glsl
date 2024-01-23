#version 460 core

out vec4 FragColor;

uniform vec4 color;

in vec3 fragToCamera;
in vec3 fragPos;
in vec3 fragNorm;

void main()
{
	float cosAngle = dot(normalize(fragToCamera), vec3(0.0, 0.0, -1.0));
	float brightness = 1.0 - cosAngle;

	brightness = clamp(brightness, 0.0, 1.0);

	FragColor = vec4(color.x * brightness, color.y * brightness, color.z * brightness, color.w);
} 