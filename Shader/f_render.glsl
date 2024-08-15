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
    sampler2DShadow shadowMap;
    mat4 lightSpaceMatrix;
};


out vec4 FragColor;

uniform Material material;

uniform int lightCount;
uniform Light lights[10];

uniform vec3 viewPos;

uniform sampler2D diffuse;

in vec3 fragPos;
in vec3 fragNorm;
in vec2 fragUv;
in vec4 worldPos;


vec2 poissonDisk[16] = vec2[]( 
   vec2( -0.94201624, -0.39906216 ), 
   vec2( 0.94558609, -0.76890725 ), 
   vec2( -0.094184101, -0.92938870 ), 
   vec2( 0.34495938, 0.29387760 ), 
   vec2( -0.91588581, 0.45771432 ), 
   vec2( -0.81544232, -0.87912464 ), 
   vec2( -0.38277543, 0.27676845 ), 
   vec2( 0.97484398, 0.75648379 ), 
   vec2( 0.44323325, -0.97511554 ), 
   vec2( 0.53742981, -0.47373420 ), 
   vec2( -0.26496911, -0.41893023 ), 
   vec2( 0.79197514, 0.19090188 ), 
   vec2( -0.24188840, 0.99706507 ), 
   vec2( -0.81409955, 0.91437590 ), 
   vec2( 0.19984126, 0.78641367 ), 
   vec2( 0.14383161, -0.14100790 ) 
);


// Returns a random number based on a vec3 and an int.
float random(vec3 seed, int i)
{
	vec4 seed4 = vec4(seed,i);
	float dot_product = dot(seed4, vec4(12.9898,78.233,45.164,94.673));
	return fract(sin(dot_product) * 43758.5453);
}

float ShadowCalculation(int lightIndex, vec4 fragPosLightSpace)
{
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    float shadow = projCoords.z < texture(lights[lightIndex].shadowMap, projCoords.xyz) ? 1.0 : 0.0;
    vec2 texelSize = 1.0 / textureSize(lights[lightIndex].shadowMap, 0);
    
    // PCF loop
    const int iterations = 3;
    for(int x = -iterations; x <= iterations; ++x)
    {
        for(int y = -iterations; y <= iterations; ++y)
        {
            vec3 offsetCoords = projCoords + vec3(x, y, 0.0) * vec3(texelSize, 0.0);
            shadow += texture(lights[lightIndex].shadowMap, offsetCoords);
        }
    }
    
    return shadow / pow(iterations * 2 + 1, 2);
}

void main()
{
    vec3 normal = normalize(fragNorm);

    vec3 lighting = material.ambient;

    for(int i = 0; i < lightCount; i++)
    {
        // diffuse
        vec3 lightDir = normalize(lights[i].position);
        float diff = max(dot(lightDir, normal), 0.0);
        vec3 diffuse = diff * lights[i].color;

        // specular
        vec3 viewDir = normalize(viewPos - fragPos);
        vec3 reflectDir = reflect(-lightDir, normal);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64.0);
        vec3 specular = vec3(0.3) * spec;

        // shadow
        vec4 fragPosLightSpace = lights[i].lightSpaceMatrix * vec4(fragPos, 1.0);
        float shadow = ShadowCalculation(i, fragPosLightSpace);

        lighting += shadow * diffuse; // + 
                    // shadow * specular;
    }

    FragColor = vec4(lighting / lightCount, 1.0);
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