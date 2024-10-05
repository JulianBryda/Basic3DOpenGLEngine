#version 460 core
struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

out vec4 FragColor;

uniform Material material;
uniform vec3 viewPos;

in vec3 fragPos;
in vec3 objPos;
in vec3 fragNorm;



void main()
{
	vec3 norm = normalize(fragNorm);

	vec3 viewDir = normalize(viewPos - fragPos);
	float dotProd = dot(norm, viewDir);
	float diff = max(dotProd, 0.0);
	
	FragColor = vec4(diff * material.diffuse, 1.0);
} 