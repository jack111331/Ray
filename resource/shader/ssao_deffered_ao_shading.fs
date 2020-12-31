#version 450 core

layout (location = 0) out float occlusion;

layout (location = 0) in vec2 fTexCoord;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D noiseTexture;

uniform vec3 samplePoints[64];

uniform int screenWidth;
uniform int screenHeight;

vec2 noiseScale = vec2(screenWidth/4.0, screenHeight/4.0);

uniform mat4 projection;

int kernelSize = 64;
float radius = 1.0;

void main()
{
    vec3 fPos = texture(gPosition, fTexCoord).rgb;
    vec3 fNormal = texture(gNormal, fTexCoord).rgb;
    vec3 fRandomVector = texture(noiseTexture, fTexCoord * noiseScale).rgb;

    vec3 tangent = normalize(fRandomVector - fNormal * dot(fRandomVector, fNormal));
    vec3 bitangent = cross(fNormal, tangent);
    mat3 TBN = mat3(tangent, bitangent, fNormal);

    occlusion = 0.0;

    for(int i = 0; i < kernelSize; ++i) {
        vec3 samplePoint = TBN * samplePoints[i];
        samplePoint = fPos + samplePoint * radius;

        vec4 offset = vec4(samplePoint, 1.0);
        offset = projection * offset;
        offset.xyz /= offset.w;
        offset.xyz = offset.xyz * 0.5 + 0.5;

        float sampleDepth = -texture(gPosition, offset.xy).w;

        float rangeCheck = smoothstep(0.0, 1.0, radius/abs(fPos.z - sampleDepth));
        occlusion += (sampleDepth >= samplePoint.z?1.0:0.0)*rangeCheck;

    }
    occlusion = 1 - (occlusion / kernelSize);
}