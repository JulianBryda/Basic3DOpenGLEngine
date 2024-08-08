#version 460 core

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct Light 
{
	vec3 position;
    vec3 color;
    sampler2D shadowMap;
    mat4 lightSpaceMatrix;
};


out vec4 FragColor;

uniform Material material;

uniform int numLights;
uniform Light lights[10];

uniform vec3 viewPos;

in vec3 fragPos;
in vec3 fragNorm;
in vec2 fragUv;


float ShadowCalculation(int lightIndex, vec4 fragPosLightSpace)
{
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    float closestDepth = texture(lights[lightIndex].shadowMap, projCoords.xy).r; 
    float currentDepth = projCoords.z;
    float shadow = currentDepth > closestDepth ? 1.0 : 0.0;

    return shadow;
}

void main()
{
    vec3 normal = normalize(fragNorm);

    vec3 lighting = material.ambient;

    for(int i = 0; i < numLights; i++)
    {
        vec3 lightDir = normalize(lights[i].position - fragPos);
        float diff = max(dot(lightDir, normal), 0.0);
        vec3 diffuse = diff * lights[i].color;

        vec3 viewDir = normalize(viewPos - fragPos);
        vec3 reflectDir = reflect(-lightDir, normal);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64.0);
        vec3 specular = vec3(0.3) * spec;

        vec4 fragPosLightSpace = lights[i].lightSpaceMatrix * vec4(fragPos, 1.0);
        float shadow = ShadowCalculation(i, fragPosLightSpace);

        lighting += (1.0 - shadow) * (diffuse + specular);
    }

    FragColor = vec4(lighting, 1.0);
}