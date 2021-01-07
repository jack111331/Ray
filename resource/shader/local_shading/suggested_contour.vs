#version 450 core
layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNormal;
layout (location = 3) in vec3 vPDir1;
layout (location = 4) in vec3 vPDir2;
layout (location = 5) in float vCurv1;
layout (location = 6) in float vCurv2;
layout (location = 7) in vec4 vDCurv;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 viewPos;

layout (location = 0) out float fNDotV;
layout (location = 1) out float fT_kr; // radiant curvature
layout (location = 2) out float fT_dwkr; // w direction derivative of radiant curvature


void main()
{
    vec3 viewDir = viewPos - vec3(model * vec4(vPos, 1.0));

    fNDotV = (1.0 / length(viewDir)) * dot(vNormal, viewDir);


    if(fNDotV > 0.0) {
        vec3 w = normalize(viewDir - vNormal * dot(viewDir, vNormal));
        float u = dot(w, vPDir1);
        float v = dot(w, vPDir2);
        float u2 = u*u;
        float v2 = v*v;
        fT_kr = (vCurv1 * u2) + (vCurv2 * v2);
        float uv = u*v;
        float dwII = (u2*u*vDCurv.x) + (3.0 * u * uv * vDCurv.y) + (3.0 * uv * v * vDCurv.z) + (v * v2 * vDCurv.w);
        fT_dwkr = dwII + 2.0 * vCurv1 * vCurv2 * fNDotV/sqrt((1.0 - pow(fNDotV, 2.0)));
    }

    gl_Position = projection * view * model * vec4(vPos, 1.0);
}