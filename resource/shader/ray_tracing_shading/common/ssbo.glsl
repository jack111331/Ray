layout(std430, binding = 0) buffer BVH {
    BVHNode bvh[];
};

layout(std430, binding = 1) buffer FlattenBVHIndices {
    uint flattenBVHIndices[];
};

layout(std430, binding = 2) buffer FlattenBVHVertices {
    vec4 flattenBVHVertices[];
};

layout(std430, binding = 3) buffer FlattenBVHNormals {
    vec4 flattenBVHNormals[];
};

layout(std430, binding = 4) buffer TransformMat {
    mat4 transformMats[];
};

layout(std430, binding = 5) buffer MaterialInstanceList {
    uint materialInstanceList[];
};

layout(std430, binding = 6) buffer LightList {
    Light lightList[];
};

layout(std430, binding = 7) buffer MaterialList {
    MaterialProperty materialList[];
};