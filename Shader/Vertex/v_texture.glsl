#version 460 core
layout (location = 0) in vec4 position;
layout (location = 2) in vec2 uv;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

out vec2 fragUv;

void main()
{
	fragUv = uv;
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * position;
}