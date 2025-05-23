#version 460 core
layout (location = 0) in vec4 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

out vec3 fragPos;
out vec3 fragNorm;
out vec2 fragUv;

void main()
{
	fragPos = (modelMatrix * position).xyz;
	fragNorm = normalize(mat3(transpose(inverse(modelMatrix))) * normal);
	fragUv = uv;

	gl_Position = projectionMatrix * viewMatrix * modelMatrix * position;
}