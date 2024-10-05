#version 460 core
layout (location = 0) in vec4 position;

out vec3 uvs;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

void main()
{
	uvs = position.xyz;
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * position;
}