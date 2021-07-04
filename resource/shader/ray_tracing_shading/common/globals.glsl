#define INFINITY  100000000.0
#define PI        3.1415926
#define EPS 0.0001

mat4 transform;

struct BVHNode {
    vec4 minBBox;
    vec4 maxBBox;
    ivec4 lrLeaf;
};

struct Ray {
    vec3 origin;
    vec3 velocity;
};

struct IntersectionRecord {
    float t;
    vec3 point;
    vec3 normal;
    vec3 ffnormal; // face-forward normal
    uint materialId;
    vec2 texCoord;
    uvec3 triangleId;
    vec3 bary;

};

struct Light {
    vec4 position;
};

struct MaterialProperty {
    uint type;
    uint padding[3];
    vec4 param0;
    vec4 param1;
    vec4 param2;
    vec4 param3;
    vec4 param4;
};

struct ONB {
    vec3 direction;
    vec3 tangent;
    vec3 cotangent;
};

bool near_zero(vec3 vector) {
    return vector.x<=1e-7&&vector.y<=1e-7&&vector.z<=1e-7;
}

vec2 seeds;

float rand(inout uint seed) {
    seed = (seed ^ 61u) ^ (seed >> 16);
    seed *= 9;
    seed = seed ^ (seed >> 4);
    seed *= 0x27d4eb2d;
    seed = seed ^ (seed >> 15);
    return seed / float(65536) / float(65536);
}

float random_in(float min, float max, inout uint seed) {
    return min + (max - min) * rand(seed);
}

vec3 random_unit_vector(inout uint seed) {
    for(int i = 0;i < 10;++i) {
        vec3 p = vec3(random_in(-1.0f, 1.0f, seed), random_in(-1.0f, 1.0f, seed), random_in(-1.0f, 1.0f, seed));
        if (length(p) >= 1) continue;
        return p;
    }
    return vec3(random_in(-1.0f, 1.0f, seed), random_in(-1.0f, 1.0f, seed), random_in(-1.0f, 1.0f, seed));
}

ONB generateONB(vec3 direction) {
    ONB onb;
    onb.direction = normalize(direction);
    vec3 up = vec3(0.0, 1.0, 0.0);
    if(onb.direction.y > 0.9) {
        up = vec3(1.0, 0.0, 0.0);
    }
    onb.tangent = cross(onb.direction, up);
    onb.cotangent = cross(onb.direction, onb.tangent);
    return onb;
}

//vec3

vec3 random_hemisphere(inout uint seed) {
    float r1 = rand(seed);
    float r2 = rand(seed);
    float x = cos(2*PI*r1)*2*sqrt(r2*(1-r2));
    float y = sin(2*PI*r1)*2*sqrt(r2*(1-r2));
    float z = 1 - r2;
    return vec3(x, y, z);
}