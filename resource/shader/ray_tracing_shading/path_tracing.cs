#version 450 core
#extension GL_ARB_shader_storage_buffer_object: enable

#include common/uniforms.glsl
#include common/globals.glsl
#include common/ssbo.glsl
#include common/intersection.glsl
#include common/closest_hit.glsl


layout(local_size_x = 16, local_size_y = 16) in;
shared vec3 tempRadiance[256];

void getNormal(inout IntersectionRecord iRecord, in Ray ray) {
    vec3 normalInTriangle[3] = {flattenBVHNormals[iRecord.triangleId.x].xyz, flattenBVHNormals[iRecord.triangleId.y].xyz, flattenBVHNormals[iRecord.triangleId.z].xyz};
    iRecord.normal = normalize(normalInTriangle[0] * iRecord.bary.x + normalInTriangle[1] * iRecord.bary.y + normalInTriangle[2] * iRecord.bary.z);
    iRecord.normal = normalize(transpose(inverse(mat3(transform))) * iRecord.normal);
    iRecord.ffnormal = dot(iRecord.normal, ray.velocity) <= 0.0f?iRecord.normal:(-1.0f * iRecord.normal);
}

vec3 path_tracing(Ray ray) {
    IntersectionRecord iRecord;
    vec4 radiance = vec4(0.0);
    vec4 attenuation = vec4(1.0);

    for(int depth = 0; depth < max_depth; ++depth) {
        float t = closestHit(ray, iRecord);
        if(t != INFINITY) {
            getNormal(iRecord, ray);
            MaterialProperty material = materialList[materialInstanceList[iRecord.materialId]];
            vec3 ffnormal = iRecord.ffnormal;
            if(material.type == 0) {
                vec3 scatter_direction = ffnormal + random_unit_vector();
                if (near_zero(scatter_direction))
                    scatter_direction = ffnormal;
                scatter_direction = normalize(scatter_direction);
                float samplePdf = dot(ffnormal, scatter_direction) / PI;
                float scatterPdf = samplePdf<0.0f?0.0f:samplePdf;
                attenuation *= material.param0 * scatterPdf / samplePdf;
                ray.velocity = scatter_direction;
                ray.origin = iRecord.point + EPS * scatter_direction;
            } else if(material.type == 2) {
                radiance += attenuation * material.param0;
                break;
            }
        } else {
            break;
        }
    }
    return radiance.rgb;
}

void main() {
    ivec2 workgroup_xy = ivec2(gl_WorkGroupID.x, gl_WorkGroupID.y);
    uint sampleId = gl_LocalInvocationID.y * gl_WorkGroupSize.x + gl_LocalInvocationID.x;

    if (sampleId < sample_per_iteration) {
        Ray ray = Ray(initial_pos, initial_vel - (workgroup_xy.x + rand()) * horizon_unit - (workgroup_xy.y + rand()) * vertical_unit);
        tempRadiance[sampleId] = path_tracing(ray);
        barrier();

        uint ib = sample_per_iteration / 2;
        while (sampleId < ib) {
            tempRadiance[sampleId] += tempRadiance[sampleId + ib];
            barrier();
            ib /= 2;
        }

        if (sampleId == 0) {
            vec4 color = texelFetch(input_color, workgroup_xy, 0) + vec4(tempRadiance[0], 1.0f);
            imageStore(output_color, workgroup_xy, vec4(color.rgb, 1.0));
        }
    }
}