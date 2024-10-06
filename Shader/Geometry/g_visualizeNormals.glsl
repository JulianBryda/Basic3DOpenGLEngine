#version 460 core
layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

in vec3 fragNorm[];

const float MAGNITUDE = 0.6;

uniform mat4 projectionMatrix;

void GenerateLine(int index)
{
    gl_Position = projectionMatrix * gl_in[index].gl_Position;
    EmitVertex();
    gl_Position = projectionMatrix * (gl_in[index].gl_Position + vec4(fragNorm[index], 0.0) * MAGNITUDE);
    EmitVertex();
    EndPrimitive();
}

void main()
{
    GenerateLine(0); // first vertex normal
    GenerateLine(1); // second vertex normal
    GenerateLine(2); // third vertex normal
}