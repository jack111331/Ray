#version 450 core
#extension GL_ARB_shader_storage_buffer_object: enable

#include common/uniforms.glsl
#include common/globals.glsl
#include common/ssbo.glsl
#include common/intersection.glsl
#include common/closest_hit.glsl


layout(local_size_x = 1, local_size_y = 1) in;

void getNormal(inout IntersectionRecord iRecord, in Ray ray) {
    vec3 normalInTriangle[3] = {flattenBVHNormals[iRecord.triangleId.x].xyz, flattenBVHNormals[iRecord.triangleId.y].xyz, flattenBVHNormals[iRecord.triangleId.z].xyz};
    iRecord.normal = normalize(normalInTriangle[0] * iRecord.bary.x + normalInTriangle[1] * iRecord.bary.y + normalInTriangle[2] * iRecord.bary.z);
    iRecord.normal = normalize(transpose(inverse(mat3(transform))) * iRecord.normal);
    iRecord.ffnormal = dot(iRecord.normal, ray.velocity) <= 0.0?iRecord.normal:-iRecord.normal;
}

vec3 whitted(Ray ray) {
    IntersectionRecord iRecord;
    vec4 finalColor = vec4(0.0);

    for(int depth = 0; depth < 6; ++depth) {
        float t = closestHit(ray, iRecord);
        if(abs(t - INFINITY) > 1e-6) {
            getNormal(iRecord, ray);
            MaterialProperty material = materialList[materialInstanceList[iRecord.materialId]];
            if(material.type == 0) {
                int lightSampleIdx = int(rand() * float(number_of_lights));
                vec3 light_origin = vec3(lightList[lightSampleIdx].position.xyz);
                vec3 light_direction = normalize(light_origin - iRecord.point);

                float ambientStrength = 0.3;
                float ambient = ambientStrength;

                // diffuse
                vec3 norm = normalize(iRecord.ffnormal);
                float diff = max(dot(norm, light_direction), 0.0);
                float diffuse = diff;

                // specular
                float specularStrength = 0.5;
                vec3 viewDir = normalize(ray.origin - iRecord.point);
                vec3 reflectDir = reflect(-light_direction, norm);
                float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
                float specular = specularStrength * spec;

                vec4 result = (ambient + diffuse + specular) * vec4(material.param0.rgb, 1.0);
                finalColor = result;
                break;
            } else if(material.type == 1) {
                float refractIndex = material.param0.r;
                ray.origin = iRecord.point + iRecord.t * 0.1 * ray.velocity;
                vec3 scatterVelocity = vec3(refract(vec4(normalize(ray.velocity), 1.0), vec4(normalize(iRecord.ffnormal), 1.0), 1.5));
                if(length(ray.velocity) < 1e-6) {
                    ray.origin = iRecord.point;
                    scatterVelocity = vec3(reflect(vec4(normalize(ray.velocity), 1.0), vec4(normalize(iRecord.ffnormal), 1.0)));
                }
                ray.velocity = scatterVelocity;
            }
        }
    }
    return finalColor.rgb;
}

void main() {
    ivec2 workgroup_xy = ivec2(gl_WorkGroupID.x, gl_WorkGroupID.y);

    Ray ray = Ray(initial_pos, initial_vel - workgroup_xy.x * horizon_unit - workgroup_xy.y * vertical_unit);

    vec3 color = whitted(ray);
    imageStore(output_color, workgroup_xy, vec4(color, 1.0));
}