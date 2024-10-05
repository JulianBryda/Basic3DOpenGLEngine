#version 460 core
layout (location = 0) in vec4 position;
layout (location = 1) in vec3 normal;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

out vec3 fragPos;
out vec3 fragNorm;

void main()
{
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * position;
	fragPos = (modelMatrix * position).xyz;
	fragNorm = normal;
}