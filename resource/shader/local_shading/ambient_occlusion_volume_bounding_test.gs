#version 450 core
layout (triangles) in;
layout(triangle_strip, max_vertices = 4) out;

void main() {
    gl_Position = gl_in[0].gl_Position;
    EmitVertex();
    gl_Position = gl_in[1].gl_Position;
    EmitVertex();
    gl_Position = gl_in[2].gl_Position;
    EmitVertex();
    gl_Position = gl_in[2].gl_Position + vec4(1.0, 1.0, 1.0, 1.0);
    EmitVertex();
    EndPrimitive();
}