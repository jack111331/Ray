#version 450 core

layout (location = 0) out vec4 finalColor;

layout (location = 0) in vec2 fTexCoord;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gColor;
uniform sampler2D gOcclusion;

struct LightAttribute {
    vec4 position;
    vec4 color;
};

layout(std140, binding = 1) uniform Lights {
    LightAttribute lights[100];
};

uniform int lightAmount;
uniform vec3 viewPos;

void main()
{
    vec3 fPos = texture(gPosition, fTexCoord).rgb;
    vec3 fNormal = texture(gNormal, fTexCoord).rgb;
    vec4 fColor = texture(gColor, fTexCoord);
    float fOcclusion = texture(gOcclusion, fTexCoord).r;
    finalColor = vec4(0.0);
    for(int i = 0; i < lightAmount; ++i) {
        // ambient
        float ambient = 0.3 * fOcclusion;

        // diffuse
        vec3 norm = normalize(fNormal);
        vec3 lightDir = normalize(vec3(lights[i].position) - fPos);
        float diff = max(dot(norm, lightDir), 0.0);
        float diffuse = diff;

        // specular
        float specularStrength = 0.5;
        vec3 viewDir = normalize(viewPos - fPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
        float specular = specularStrength * spec;

        vec4 result = (ambient + diffuse + specular) * fColor * lights[i].color;
        finalColor += result;
        finalColor = vec4(ambient, ambient, ambient, 1.0);
    }
}