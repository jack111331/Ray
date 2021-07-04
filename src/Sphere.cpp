//
// Created by Edge on 2020/7/7.
//

#include "glad/glad.h"
#include "Sphere.h"

using namespace std;

bool Sphere::isHit(const Ray &ray, IntersectionRecord &record, float tmin, const glm::mat4 &transformMat) const {
    // TODO transform matrix
    double a = ray.velocity.dot(ray.velocity);
    double b = 2 * (ray.origin - m_origin).dot(ray.velocity);
    double c = (ray.origin - m_origin).dot(ray.origin - m_origin) - m_radius * m_radius;
    double determinant = b * b - 4.0 * a * c;
    if (determinant > 0.0) {
        double t[2] = {(-b - sqrt(determinant)) / 2.0 / a, (-b + sqrt(determinant)) / 2.0 / a};
        for (double i : t) {
            if (i > tmin) {
                if (record.t < 0.0 || record.t > i) {
                    record.t = i;
                    record.point = ray.pointAt(i);
                    record.point = transformMat * record.point;
                    Vec3f transformedOrigin = transformMat * m_origin;
                    record.normal = (record.point - transformedOrigin).normalize();
                    record.ffNormal = ray.velocity.dot(record.normal)<0?record.normal:-record.normal;
                    record.material = m_material;
                    return true;
                }
            }
        }
    }
    return false;
}

void Sphere::createVAO(std::vector<ShadeObject *> &shadeObjectList, const glm::mat4 &transformMat) {
    int stacks = 20;
    int slices = 20;
    const float PI = 3.14f;

    std::vector<float> positions;
    std::vector<float> normals;
    std::vector<uint32_t> indices;

    // loop through stacks.
    for (int i = 0; i <= stacks; ++i) {
        float V = (float) i / (float) stacks;
        float phi = V * PI;

        // loop through the slices.
        for (int j = 0; j <= slices; ++j) {

            float U = (float) j / (float) slices;
            float theta = U * (PI * 2);

            // use spherical coordinates to calculate the positions.
            float x = cos(theta) * sin(phi);
            float y = cos(phi);
            float z = sin(theta) * sin(phi);

            normals.push_back(x);
            normals.push_back(y);
            normals.push_back(z);

            x = m_origin.x + m_radius * x;
            y = m_origin.y + m_radius * y;
            z = m_origin.z + m_radius * z;

            positions.push_back(x);
            positions.push_back(y);
            positions.push_back(z);
        }
    }

    // Calc The Index Positions
    for (int i = 0; i < slices * stacks + slices; ++i) {
        indices.push_back(i);
        indices.push_back(i + slices + 1);
        indices.push_back(i + slices);

        indices.push_back(i + slices + 1);
        indices.push_back(i);
        indices.push_back(i + 1);
    }

    // VAO
    uint32_t vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Mesh VBO
    uint32_t meshVbo;
    glGenBuffers(1, &meshVbo);
    glBindBuffer(GL_ARRAY_BUFFER, meshVbo);
    glBufferData(GL_ARRAY_BUFFER, (stacks + 1) * (slices + 1) * 3 * sizeof(float), positions.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    // Normal VBO
    uint32_t normalVbo;
    glGenBuffers(1, &normalVbo);
    glBindBuffer(GL_ARRAY_BUFFER, normalVbo);
    glBufferData(GL_ARRAY_BUFFER, (stacks + 1) * (slices + 1) * 3 * sizeof(float), normals.data(), GL_DYNAMIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(1);

    // EBO
    uint32_t ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * (slices * stacks + slices) * sizeof(uint32_t), indices.data(),
                 GL_STATIC_DRAW);

    glBindVertexArray(0);

    shadeObjectList.push_back(new ShadeObject({vao, 6 * (slices * stacks + slices)}, m_material, transformMat));
}

bool Sphere::readObjectInfo(const YAML::Node &node, const Scene *scene) {
    bool result = BLASNode::readObjectInfo(node, scene);
    result = min(result, node["position"] && node["radius"]);
    if (!result) {
        std::cerr << "No require sphere node" << std::endl;
        return false;
    }
    m_origin = Vec3f::toVec3f(node["position"].as<std::vector<float>>());
    m_radius = node["radius"].as<float>();
    return result;
}

ObjectBoundingBox Sphere::getBoundingBox() const {
    return ObjectBoundingBox(m_origin - Vec3f{m_radius, m_radius, m_radius},
                             m_origin + Vec3f{m_radius, m_radius, m_radius});
}
