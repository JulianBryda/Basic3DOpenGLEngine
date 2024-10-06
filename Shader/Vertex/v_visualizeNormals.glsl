#version 460 core
layout (location = 0) in vec4 position;
layout (location = 1) in vec3 normal;

uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

out vec3 fragNorm;

void main()
{
	mat3 normalMatrix = mat3(transpose(inverse(viewMatrix * modelMatrix)));
    fragNorm = vec3(vec4(normalMatrix * normal, 0.0));
	gl_Position = viewMatrix * modelMatrix * position;
}