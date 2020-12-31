#version 450 core

out vec4 finalColor;

in vec3 fPos;
in vec3 fNormal;

struct LightAttribute {
    vec4 position;
    vec4 color;
};

layout(std140, binding = 1) uniform Lights {
    LightAttribute lights[100];
};

uniform int lightAmount;

uniform vec3 viewPos;
uniform vec3 objectColor;

void main()
{
    finalColor = vec4(0.0);
    for(int i = 0; i < lightAmount; ++i) {
        // ambient
        float ambientStrength = 0.1;
        float ambient = ambientStrength;

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

        vec4 result = (ambient + diffuse + specular) * vec4(objectColor, 1.0) * lights[i].color;
        finalColor += result;
    }
}