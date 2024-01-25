#version 460 core

out vec4 FragColor;

uniform vec4 color;
uniform vec3 camPos;

in vec3 fragPos;
in vec3 fragNorm;

void main()
{
    float diff = max(dot(normalize(fragNorm), normalize(camPos - fragPos)), 0.0);
    vec3 diffuse = diff * vec3(1.0) * color.rgb;

    FragColor = vec4(diffuse, 1.0);
} 