#version 460 core
layout (location = 0) in vec4 aPos;

out vec3 uvs;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
	uvs = aPos.xyz;
	gl_Position = projection * view * model * aPos;
}