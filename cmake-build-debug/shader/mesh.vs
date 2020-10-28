#version 450 core
layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNormal;

out vec3 fPos;
out vec3 fNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    fPos = vec3(model * vec4(vPos, 1.0));
    fNormal = mat3(transpose(inverse(model))) * vNormal;

    gl_Position = projection * view * vec4(fPos, 1.0);
}