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
out vec4 worldPos;

void main()
{
	fragPos = (model * position).xyz;
	fragNorm = mat3(transpose(inverse(model))) * normal;
	fragUv = uv;

	worldPos = projection * view * model * position;

	gl_Position = worldPos;
}