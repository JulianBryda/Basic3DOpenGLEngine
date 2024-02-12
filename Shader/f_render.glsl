#version 460 core
struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct DirectionalLight 
{
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight 
{
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

struct SpotLight 
{
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};


out vec4 FragColor;

uniform Material material;
uniform DirectionalLight directionalLight;
uniform PointLight pointLights[100];

uniform vec3 viewPos;
uniform float pointLightCount;

in vec3 fragPos;
in vec3 fragNorm;
in vec2 fragUv;


vec3 CalcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // combine results
    vec3 ambient = light.ambient * material.ambient;
    vec3 diffuse = light.diffuse * material.diffuse;
    vec3 specular = light.specular * spec * material.specular;
    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
  			     light.quadratic * (distance * distance));    
    // combine results
    vec3 ambient = light.ambient * material.ambient;
    vec3 diffuse = light.diffuse * diff * material.diffuse;
    vec3 specular = light.specular * spec * material.specular;
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
} 



void main()
{
    vec3 norm = normalize(fragNorm);
    vec3 viewDir = normalize(viewPos - fragPos);

    vec3 result = CalcDirLight(directionalLight, norm, viewDir);

    for (int i = 0; i < pointLightCount; i++)
    {
        result += CalcPointLight(pointLights[i], norm, fragPos, viewDir);
    }

    FragColor = vec4(result, 1.0);
} 