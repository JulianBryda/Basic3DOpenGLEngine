#version 460 core
layout (location = 0) in vec4 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

out vec3 fragPos;
out vec3 fragNorm;
out vec2 fragUv;
out mat3 tbnMatrix;

void main()
{
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * position;
	fragPos = (modelMatrix * position).xyz;
	fragNorm = normal;
	fragUv = uv;
	tbnMatrix = transpose(mat3(tangent, bitangent, normal));
}