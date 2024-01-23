#version 460 core
layout (location = 0) in vec4 position;
layout (location = 1) in vec3 normal;

uniform mat4 model;
uniform mat4 scale;
uniform mat4 projection;
uniform mat4 view;

uniform vec3 camPos;

out vec3 fragPos;
out vec3 fragNorm;
out vec3 fragToCamera;

void main()
{
	gl_Position = projection * view * model * scale * position;
	fragPos = (model * scale * position).xyz;
	fragNorm = normal;

	vec4 fragToCameraView = view * vec4(camPos - position.xyz, 1.0);
	fragToCamera = fragToCameraView.xyz / fragToCameraView.w;
}