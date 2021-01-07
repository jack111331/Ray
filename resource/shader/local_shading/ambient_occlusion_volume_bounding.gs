#version 450 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 12) out;

out GS_OUT {
    flat vec3 sourceVertex0, sourceVertex1, sourceVertex2;
    flat float area;
    flat vec3 m0, m1, m2, topNormal;
} gs_out;

uniform mat4 worldToScreenMatrix;
uniform vec3 viewPos;
uniform float maxObscuranceDistance;

vec3 edgeNormal[3];
vec3 negFaceNormal;
float sharedArea;

void emit(vec4 vertex) {
    gl_Position = vertex;

    // Even though the following are the same for each vertex, we have
    // to re-emit them because outputs are undefined after EmitVertex.

    // Copy over the original vertex positions and inward-facing volume face normals
    gs_out.sourceVertex0 = gl_in[0].gl_Position.xyz;
    gs_out.m0 = edgeNormal[0];

    gs_out.sourceVertex1 = gl_in[1].gl_Position.xyz;
    gs_out.m1 = edgeNormal[1];

    gs_out.sourceVertex2.xyz = gl_in[2].gl_Position.xyz;
    gs_out.area = sharedArea;
    gs_out.m2 = edgeNormal[2];

    gs_out.topNormal = negFaceNormal;

    EmitVertex();
}

void main()
{
    negFaceNormal = -cross(gl_in[1].gl_Position.xyz - gl_in[0].gl_Position.xyz, gl_in[2].gl_Position.xyz - gl_in[0].gl_Position.xyz);
    sharedArea = length(negFaceNormal);
    negFaceNormal = normalize(negFaceNormal);

    for(int i = 2, j = 0;j < 3;i = j++) {
        edgeNormal[i] = normalize(cross(gl_in[j].gl_Position.xyz - gl_in[i].gl_Position.xyz, negFaceNormal));
    }

    vec4 v[6];
    float maxLen = maxObscuranceDistance * 2.0;

    float d = dot(gl_in[0].gl_Position.xyz - viewPos, negFaceNormal);

    bool inside = (d>0.0) && (d<maxObscuranceDistance);

    float maxLen2 = maxLen * maxLen;
    for(int i = 2, j=0;j < 3;i = j++) {
        mat3 M = mat3(edgeNormal[i], edgeNormal[j], negFaceNormal);
        vec3 offset = vec3(-maxObscuranceDistance, -maxObscuranceDistance, 0.0) * inverse(M);
        float len2 = dot(offset, offset);
        if(len2 > maxLen2) {
            offset *= maxLen * inversesqrt(len2);
        }
        v[j].xyz = gl_in[j].gl_Position.xyz + offset;
        inside = inside && (dot(v[j].xyz - viewPos, edgeNormal[j]) < 0.0);

        v[j+3] = worldToScreenMatrix * vec4(negFaceNormal * -maxObscuranceDistance + v[j].xyz, 1.0);
        v[j] = worldToScreenMatrix * vec4(v[j].xyz, 1.0);
    }
    // test camera is in bounding box
    if (inside) {
        emit(vec4(-1.0, -1.0, 0.0, 1.0));
        emit(vec4(-1.0, 1.0, 0.0, 1.0));
        emit(vec4(1.0, -1.0, 0.0, 1.0));
        emit(vec4(1.0, 1.0, 0.0, 1.0));
    } else {
        emit(v[3]);
        emit(v[4]);
        emit(v[5]);

        emit(v[1]);
        emit(v[2]);

        emit(v[0]);
        EndPrimitive();

        emit(v[5]);
        emit(v[2]);
        emit(v[3]);
        emit(v[0]);

        emit(v[4]);
        emit(v[1]);
    }
    EndPrimitive();
}