#version 460 core
layout (location = 0) in vec4 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec3 fragPos;
out vec3 fragNorm;
out vec2 fragUv;

void main()
{
	gl_Position = projection * view * model * position;
	fragPos = (model * position).xyz;
	fragNorm = normal;
	fragUv = uv;
}