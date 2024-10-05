#version 460 core

layout(binding = 0, r32ui) uniform uimage2D atomicImage;

out vec4 FragColor;

void main()
{
    ivec2 coord = ivec2(gl_FragCoord.xy);
    uint count = imageAtomicAdd(atomicImage, coord, 1);

    vec3 color;

    if (count == 0)
    {
        color = vec3(0.0);
    }
    else if (count == 1)
    {
        color = vec3(0.0, 0.0, 1.0);
    } 
    else if (count == 2)
    {
        color = vec3(0.0, 1.0, 1.0);
    }
    else if (count == 3)
    {
        color = vec3(0.0, 1.0, 0.0);
    }
    else 
    {
        color = vec3(1.0, 1.0, 0.0);
    }


    FragColor = vec4(color, 1.0);
}
