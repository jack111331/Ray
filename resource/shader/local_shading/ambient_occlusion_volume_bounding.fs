#version 450 core

layout (location = 0) out float occlusion;
in GS_OUT {
    flat vec3 sourceVertex0, sourceVertex1, sourceVertex2;
    flat float area;
    flat vec3 m0, m1, m2, topNormal;
} fs_in;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform float invMaxObscuranceDistance;
uniform float falloffExponent;

float projArea(const in vec3 a, const in vec3 b, const in vec3 n) {
    vec3 bXa = cross(b, a);
    float cosine = dot(a, b);
    float theta = acos(cosine * inversesqrt(dot(a, a) * dot(b, b)));
    return theta * dot(n, bXa) * inversesqrt(dot(bXa, bXa));
}

const float epsilon = 0.00001;

bool clipToPlane(const in vec3 n, inout vec3 v0, inout vec3 v1, inout vec3 v2, out vec3 v3) {

    vec3 dist = vec3(dot(v0, n), dot(v1, n), dot(v2, n));

    bool quad = false;

    if (! any(greaterThanEqual(dist, vec3(0.01)))) {
        discard;
    } else if (all(greaterThanEqual(dist, vec3(-epsilon)))) {
    } else {
        bvec3 above = greaterThanEqual(dist, vec3(0.0));

        bool nextIsAbove;

        if (above[1] && ! above[0]) {
            nextIsAbove = above[2];
            v3 = v0; v0 = v1; v1 = v2; v2 = v3;
            dist = dist.yzx;
        } else if (above[2] && ! above[1]) {
            nextIsAbove = above[0];
            v3 = v2; v2 = v1; v1 = v0; v0 = v3;
            dist = dist.zxy;
        } else {
            nextIsAbove = above[1];
        }
        v3 = mix(v0, v2, dist[0] / (dist[0] - dist[2]));

        if (nextIsAbove) {
            quad = true;
            v2 = mix(v1, v2, dist[1] / (dist[1] - dist[2]));

        } else {
            v2 = v3;
            v1 = mix(v0, v1, dist[0] / (dist[0] - dist[1]));
        }
    }

    v3 = quad ? v3 : v0;

    return quad;
}

float computeFalloffWeight(in vec3 origin, out vec3 p0, out vec3 p1, out vec3 p2) {

    vec4 pm;
    p0  = fs_in.sourceVertex0 - origin;

    pm.w = dot(p0, fs_in.topNormal);

    if ((pm.w < epsilon) || (fs_in.area < pm.w * 0.3)) {
        discard;
    }

    pm.x = dot(p0, fs_in.m0);

    p1  = fs_in.sourceVertex1 - origin;
    pm.y = dot(p1, fs_in.m1);

    p2  = fs_in.sourceVertex2 - origin;
    pm.z = dot(p2, fs_in.m2);

    vec4 g = clamp(vec4(1.0) - pm * invMaxObscuranceDistance, vec4(0.0), vec4(1.0));

    g.w = pow(g.w, falloffExponent);

    float f = g.x * g.y * g.z * g.w;

    if (f < 0.1) {
        discard;
    }

    return f;
}

float computeFormFactor(vec3 n, in vec3 p0, in vec3 p1, in vec3 p2) {
    vec3 p3;
    bool quad = clipToPlane(n, p0, p1, p2, p3);
    float result = 0.0;
    if (quad) {
        result += projArea(p3, p0, n);
    }

    result += projArea(p0, p1, n);
    result += projArea(p1, p2, n);
    result += projArea(p2, p3, n);

    const float adjust = 1.0 / (2.0 * 3.1415927);
    return result * adjust;
}

void main() {
    ivec2 iFragCoord = ivec2(gl_FragCoord.xy);

    vec3 p0, p1, p2;

    vec3 x = texelFetch(gPosition, iFragCoord, 0).xyz;

    float falloffWeight = computeFalloffWeight(x, p0, p1, p2);

    vec3 n = texelFetch(gNormal, iFragCoord, 0).xyz;
    float formFactor = computeFormFactor(n, p0, p1, p2);

    formFactor = clamp(formFactor * falloffWeight, 0.0, 1.0);

    occlusion = formFactor;
}
