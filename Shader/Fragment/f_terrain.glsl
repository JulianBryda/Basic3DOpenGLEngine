#version 460 core

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

out vec4 FragColor;

uniform vec3 flatColor;
uniform vec3 slopeColor;

uniform float slopeStart;
uniform float slopeEnd;

uniform vec3 viewPos;

in vec3 fragPos;
in vec3 fragNorm;

void main()
{
	vec3 upVector = vec3(0.0, 1.0, 0.0);

	float slopeFactor = dot(fragNorm, upVector);
	slopeFactor = (slopeFactor - slopeStart) / (slopeEnd - slopeStart);
	slopeFactor = clamp(slopeFactor, 0.0, 1.0);

	vec3 finalColor = mix(slopeColor, flatColor, slopeFactor);

	vec3 viewDir = normalize(viewPos - fragPos);
	float dotProd = dot(fragNorm, viewDir);
	float diff = max(dotProd, 0.0);

	FragColor = vec4(diff * finalColor, 1.0);
}