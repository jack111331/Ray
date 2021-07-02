#version 450 core

layout (location = 0) out vec4 finalColor;

layout (location = 0) in vec2 fTexCoord;

uniform sampler2D gScreen;

void main()
{
    finalColor = texture(gScreen, vec2(fTexCoord.x, 1.0-fTexCoord.y));
}