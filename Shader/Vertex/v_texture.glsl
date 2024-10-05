#version 460 core
layout (location = 0) in vec4 pos;
layout (location = 2) in vec2 uv;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

out vec2 fragUv;

void main()
{
	gl_Position = projection * view * model * pos;
	fragUv = uv;
}