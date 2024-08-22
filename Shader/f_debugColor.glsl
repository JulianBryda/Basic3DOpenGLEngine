#version 330 core

out vec4 FragColor;

float rand(int seed) {
    return fract(sin(float(seed) * 78.233) * 43758.5453123);
}

void main()
{
    vec3 color = vec3(
        rand(gl_PrimitiveID * 17), // Red
        rand(gl_PrimitiveID * 31), // Green
        rand(gl_PrimitiveID * 47)  // Blue
    );

    FragColor = vec4(color, 1.0); // Set the output color of the pixel
}
