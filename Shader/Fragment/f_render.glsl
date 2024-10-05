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
    vec3 direction;
    sampler2D shadowMap;
    vec3 color;
    mat4 lightSpaceMatrix;
    int lightType;
    float innerCone;
    float outerCone;
};

const int LIGHT_POINT = 0;
const int LIGHT_DIRECTIONAL = 1;
const int LIGHT_SPOT = 2;



out vec4 FragColor;

uniform Material material;
uniform sampler2D diffuseTexture;

uniform int lightCount;
uniform Light lights[10];

uniform vec3 viewPos;

in vec3 fragPos;
in vec3 fragNorm;
in vec2 fragUv;

float ShadowCalculation(int lightIndex, vec4 fragPosLightSpace, float biasLimitMin, float biasLimitMax)
{
    float shadow = 0.0;
    vec3 lightCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    if (lightCoords.z <= 1.0)
    {
        lightCoords = (lightCoords + 1.0) / 2.0;
        float currentDepth = lightCoords.z;
        float bias = max(biasLimitMin * (1.0 - dot(fragNorm, lights[lightIndex].position)), biasLimitMax);

        int sampleRadius = 2;
        vec2 pixelSize = 1.0 / textureSize(lights[lightIndex].shadowMap, 0);
        for(int y = -sampleRadius; y <= sampleRadius; y++)
        {
            for(int x = -sampleRadius; x <= sampleRadius; x++)
            {
                float closestDepth = texture(lights[lightIndex].shadowMap, lightCoords.xy + vec2(x, y) * pixelSize).r;
                if (currentDepth > closestDepth + bias)
                    shadow += 1.0;
            }
        }

        shadow /= pow((sampleRadius * 2 + 1), 2);
    }

    return shadow;
}

vec4 Spot(vec3 lightPosition, vec3 lightDirection, vec3 lightColor, float innerCone, float outerCone)
{
    // diffuse
    vec3 lightDir = normalize(lightPosition - fragPos);
    float diff = max(dot(lightDir, fragNorm), 0.0);
    vec4 diffuse = vec4(lightColor * material.diffuse, 1.0);
    diffuse = (diffuse * texture(diffuseTexture, fragUv)) * diff;

    // specular
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, fragNorm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec4 specular = vec4(spec * (lightColor * material.specular), 1.0);

    float angle = dot(lightDirection, -lightDir);
    float inten = clamp((angle - outerCone) / (innerCone - outerCone), 0.0, 1.0);

    return (diffuse * inten) + (specular * inten);
}

vec4 Directional(vec3 lightDirection, vec3 lightColor)
{
    // diffuse
    vec3 lightDir = normalize(lightDirection);
    float diff = max(dot(lightDir, fragNorm), 0.0);
    vec4 diffuse = vec4(lightColor * material.diffuse, 1.0);
    diffuse = (diffuse * texture(diffuseTexture, fragUv)) * diff;
    
    // specular
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, fragNorm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec4 specular = vec4(spec * (lightColor * material.specular), 1.0);

    return diffuse + specular;
}



void main()
{
    vec4 lighting = vec4(material.ambient * material.diffuse, 1.0);

    for(int i = 0; i < lightCount; i++)
    {
        // shadow
        vec4 fragPosLightSpace = lights[i].lightSpaceMatrix * vec4(fragPos, 1.0);
        float shadow = 0.0;

        if (lights[i].lightType == LIGHT_DIRECTIONAL)
        {
            shadow = ShadowCalculation(i, fragPosLightSpace, 0.025, 0.0027);
            lighting += (1.0 - shadow) * Directional(lights[i].direction, lights[i].color);
        }
        else if (lights[i].lightType == LIGHT_SPOT)
        {
            shadow = ShadowCalculation(i, fragPosLightSpace, 0.00025, 0.000027);
            lighting += (1.0 - shadow) * Spot(lights[i].position, lights[i].direction, lights[i].color, lights[i].innerCone, lights[i].outerCone);
        }
    }

    FragColor = lighting;
}


// test code

//void main()
//{
//    vec3 lighting = vec3(0.1);
//
//    for(int i = 0; i < lightCount; i++)
//    {
//        vec4 fragPosLightSpace = lights[i].lightSpaceMatrix * vec4(fragPos, 1.0);
//        float shadow = ShadowCalculation(i, fragPosLightSpace);
//
//        lighting += shadow * vec3(1.0);
//    }
//
//    FragColor = vec4(lighting, 1.0);
//}