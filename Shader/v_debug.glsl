#version 460 core
layout (location = 0) in vec4 position;
layout (location = 1) in vec3 normal;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec3 fragPos;
out vec3 fragNorm;

void main()
{
	gl_Position = projection * view * model * position;
	fragPos = (model * position).xyz;
	fragNorm = normal;
}