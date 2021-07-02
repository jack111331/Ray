
// Reference from https://github.com/knightcrawler25/GLSL-PathTracer/blob/master/src/shaders/common/closest_hit.glsl
float closestHit(Ray ray, inout IntersectionRecord iRecord) {
    float t = INFINITY;

    int stack[64];
    int ptr = 0;
    stack[ptr++] = -1;

    int idx = top_level_bvh_index;
    float leftHit = 0.0f;
    float rightHit = 0.0f;

    bool isBLASNode = false;

    Ray ray_transformed = ray;
    mat4 temp_transform;

    int current_material_id = 0;

    while(idx > -1 || isBLASNode) {
        if(idx < 0 && isBLASNode) {
            ray_transformed.origin = ray.origin;
            ray_transformed.velocity = ray.velocity;
            isBLASNode = false;
            idx = stack[--ptr];
            continue;
        }
        int index = idx;
        ivec4 LRLeaf = bvh[index].lrLeaf;

        int leftIndex = int(LRLeaf.x);
        int rightIndex = int(LRLeaf.y);
        int leaf = int(LRLeaf.z);
        // Leaf Node of BLAS
        if(leaf > 0) {
            for(int i = 0; i < 3 * rightIndex; i += 3) {
                int index = leftIndex+i;
                uvec3 vert_indices;
                vert_indices.x = flattenBVHIndices[index];
                vert_indices.y = flattenBVHIndices[index+1];
                vert_indices.z = flattenBVHIndices[index+2];

                vec3 v0 = vec3(flattenBVHVertices[vert_indices.x].xyz);
                vec3 v1 = vec3(flattenBVHVertices[vert_indices.y].xyz);
                vec3 v2 = vec3(flattenBVHVertices[vert_indices.z].xyz);

                vec3 e0 = v1 - v0;
                vec3 e1 = v2 - v0;
                vec3 pv = cross(ray_transformed.velocity, e1);
                float det = dot(e0, pv);

                vec3 tv = ray_transformed.origin - v0;
                vec3 qv = cross(tv, e0);

                vec4 uvt;
                uvt.x = dot(tv, pv);
                uvt.y = dot(ray_transformed.velocity, qv);
                uvt.z = dot(e1, qv);
                uvt.xyz = uvt.xyz / det;
                uvt.w = 1.0 - uvt.x - uvt.y;
                if(all(greaterThanEqual(uvt, vec4(0.0f))) && uvt.z < t) {
                    t = uvt.z;
                    vec3 normal;
                    int material;
                    vec2 texCoord;
                    iRecord.point = ray_transformed.origin + ray_transformed.velocity * t;
                    iRecord.point = vec3(temp_transform * vec4(iRecord.point, 1.0));
                    iRecord.triangleId = vert_indices;
                    iRecord.bary = uvt.xyz;
                    iRecord.materialId = current_material_id;
                    transform = temp_transform;
                    // TODO other property
                }
            }
        } else if (leaf < 0) {
            // Leaf node of Top-Level Acceleration Structure
            // Handle transform group
            current_material_id = -leaf-1;
            temp_transform = transformMats[-leaf-1];
            ray_transformed.origin = vec3(inverse(temp_transform) * vec4(ray.origin, 1.0));
            ray_transformed.velocity = vec3(inverse(temp_transform) * vec4(ray.velocity, 0.0));

            stack[ptr++] = -1;
            isBLASNode = true;
            idx = leftIndex;
            continue;
        } else {
            // Internal node of TLAS and BLAS
            // Left node just handle geometry group
            leftHit = AABBIntersect(vec3(bvh[leftIndex].minBBox.xyz), vec3(bvh[leftIndex].maxBBox.xyz), ray_transformed);
            rightHit = AABBIntersect(vec3(bvh[rightIndex].minBBox.xyz), vec3(bvh[rightIndex].maxBBox.xyz), ray_transformed);

            if(leftHit > 0.0f && rightHit > 0.0f) {
                // deferred determine whether left or right tree will test first
                int deferred = -1;
                if(leftHit > rightHit) {
                    idx = rightIndex;
                    deferred = leftIndex;
                } else {
                    idx = leftIndex;
                    deferred = rightIndex;
                }
                stack[ptr++] = deferred;
                continue;
            } else if(leftHit > 0.0f) {
                idx = leftIndex;
                continue;
            } else if(rightHit > 0.0f) {
                idx = rightIndex;
                continue;
            }
        }
        idx = stack[--ptr];
    }
    iRecord.t = t;
    return t;
}