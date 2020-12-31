#version 450 core

layout (location = 0) out vec3 gPos;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gColor;

layout (location = 0) in vec3 fPos;
layout (location = 1) in vec3 fNormal;
layout (location = 2) in vec4 fColor;

const float NEAR = 0.1;
const float FAR = 50.0;

void main()
{
    gPos = fPos;
    gNormal = normalize(fNormal);
    gColor = fColor;
}