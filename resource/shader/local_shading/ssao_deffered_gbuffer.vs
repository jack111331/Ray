#version 450 core
layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNormal;

layout(location = 0) out vec3 fPos;
layout(location = 1) out vec3 fNormal;
layout(location = 2) out vec4 fColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 objectColor;

void main()
{
    fPos = vec3(view * model * vec4(vPos, 1.0));
    fNormal = mat3(transpose(inverse(view * model))) * vNormal;
    fColor = vec4(objectColor, 1.0);

    gl_Position = projection * vec4(fPos, 1.0);
}