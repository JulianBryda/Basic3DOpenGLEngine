#version 460 core

out vec4 FragColor;

uniform vec4 color;
uniform sampler2D albedo;

in vec2 fragUv;

void main()
{
    FragColor = texture(albedo, fragUv) * color;
} 