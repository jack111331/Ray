#version 450 core

layout (location = 0) out vec4 finalColor;

layout (location = 0) in float fNDotV;
layout (location = 1) in float fT_kr; // radiant curvature
layout (location = 2) in float fT_dwkr; // w direction derivative of radiant curvature

uniform float fz;
uniform float c_limit;
uniform float sc_limit;
uniform float dwkr_limit;

void main()
{
    finalColor = vec4(1.0);
    float kr = fz * abs(fT_kr);
    float dwkr = fz*fz*fT_dwkr;
    float dwkr2 = (dwkr-dwkr*pow(fNDotV, 2.0));

    float contour_limit = c_limit*(pow(fNDotV, 2.0)/kr);
    float sc_limit = sc_limit * (kr/dwkr2);

    if(contour_limit < 1.0) {
        finalColor.rgb = min(finalColor.rgb, vec3(contour_limit));
    } else if((sc_limit < 1.0) && dwkr2 > dwkr_limit) {
        finalColor.rgb = min(finalColor.rgb, vec3(sc_limit));
    }
    finalColor.rgb = vec3(0.7, 0.5, 0.9) * finalColor.rgb;
}