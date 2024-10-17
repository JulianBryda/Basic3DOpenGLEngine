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

float ShadowCalculation(Light light, vec4 fragPosLightSpace, float biasLimitMin, float biasLimitMax, vec3 fragNorm)
{
    float shadow = 0.0;
    vec3 lightCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    if (lightCoords.z <= 1.0)
    {
        lightCoords = (lightCoords + 1.0) / 2.0;
        float currentDepth = lightCoords.z;
        float bias = max(biasLimitMin * (1.0 - dot(fragNorm, light.position)), biasLimitMax);

        int sampleRadius = 2;
        vec2 pixelSize = 1.0 / textureSize(light.shadowMap, 0);
        for(int y = -sampleRadius; y <= sampleRadius; y++)
        {
            for(int x = -sampleRadius; x <= sampleRadius; x++)
            {
                float closestDepth = texture(light.shadowMap, lightCoords.xy + vec2(x, y) * pixelSize).r;
                if (currentDepth > closestDepth + bias)
                    shadow += 1.0;
            }
        }

        shadow /= pow((sampleRadius * 2 + 1), 2);
    }

    return shadow;
}

vec4 Spot(vec3 lightPosition, vec3 lightDirection, vec3 lightColor, float innerCone, float outerCone, vec3 fragPos, vec3 fragNorm, vec3 viewPos, vec3 diffuseColor, vec3 specularColor, float shininess)
{
    // diffuse
    vec3 lightDir = normalize(lightPosition - fragPos);
    float diff = max(dot(lightDir, fragNorm), 0.0);
    vec4 diffuse = vec4(diff * (lightColor * diffuseColor), 1.0);

    // specular
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, fragNorm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec4 specular = vec4(spec * (lightColor * specularColor), 1.0);

    float angle = dot(lightDirection, -lightDir);
    float inten = clamp((angle - outerCone) / (innerCone - outerCone), 0.0, 1.0);

    return (diffuse * inten) + (specular * inten);
}

vec4 Directional(vec3 lightDirection, vec3 lightColor, vec3 fragPos, vec3 fragNorm, vec3 viewPos, vec3 diffuseColor, vec3 specularColor, float shininess)
{
    // diffuse
    vec3 lightDir = normalize(lightDirection);
    float diff = max(dot(lightDir, fragNorm), 0.0);
    vec4 diffuse = vec4(diff * (lightColor * diffuseColor), 1.0);
    
    // specular
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, fragNorm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec4 specular = vec4(spec * (lightColor * specularColor), 1.0);

    return diffuse + specular;
}

vec4 CalculateLighting(Light lights[10], int lightCount, vec3 fragPos, vec3 fragNorm, vec3 viewPos, vec3 ambient, vec3 diffuse, vec3 specular, float shininess)
{
    vec4 lighting = vec4(ambient * diffuse, 1.0);

    for (int i = 0; i < lightCount; i++)
    {
        // shadow
        vec4 fragPosLightSpace = lights[i].lightSpaceMatrix * vec4(fragPos, 1.0);
        float shadow = 0.0;

        if (lights[i].lightType == LIGHT_DIRECTIONAL)
        {
            shadow = ShadowCalculation(lights[i], fragPosLightSpace, 0.025, 0.0027, fragNorm);
            lighting += (1.0 - shadow) * Directional(lights[i].direction, lights[i].color, fragPos, fragNorm, viewPos, diffuse, specular, shininess);
        }
        else if (lights[i].lightType == LIGHT_SPOT)
        {
            shadow = ShadowCalculation(lights[i], fragPosLightSpace, 0.00025, 0.000027, fragNorm);
            lighting += (1.0 - shadow) * Spot(lights[i].position, lights[i].direction, lights[i].color, lights[i].innerCone, lights[i].outerCone, fragPos, fragNorm, viewPos, diffuse, specular, shininess);
        }
    }

    return lighting;
}