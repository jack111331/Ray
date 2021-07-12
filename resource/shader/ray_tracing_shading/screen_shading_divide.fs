#version 450 core

layout (location = 0) out vec4 finalColor;

layout (location = 0) in vec2 fTexCoord;

uniform sampler2D gScreen;

uniform int casted_ray;

void main() {
    float casted_ray_amount = casted_ray;
    vec4 color = texture(gScreen, vec2(fTexCoord.x, 1.0f - fTexCoord.y)) / casted_ray_amount;
    color = clamp(color, 0.0f, 1.0f);
//    finalColor = vec4(color.rgb, 1.0f);
// FIXME
    finalColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}