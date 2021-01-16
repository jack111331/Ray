//
// Created by Edge on 2020/7/9.
//

#include "Triangle.h"

#include <cmath>
#include <GL/glew.h>

using namespace std;

bool Triangle::isHit(const Ray &ray, IntersectionRecord &record, float tmin, const glm::mat4 &transformMat) const {
    const double EPSILON = 1e-7;
    Vec3f planeVector[2] = {m_point[1]->m_coord - m_point[0]->m_coord, m_point[2]->m_coord - m_point[0]->m_coord};
    Vec3f h = ray.velocity.cross(planeVector[1]);
    float a = planeVector[0].dot(h);
    if (std::abs(a) < EPSILON)
        return false;
    float f = 1.0 / a;
    Vec3f s = ray.origin - m_point[0]->m_coord;
    float u = f * s.dot(h);
    if (u < 0.0 || u > 1.0)
        return false;
    Vec3f q = s.cross(planeVector[0]);
    float v = f * ray.velocity.dot(q);
    if (v < 0.0 || v + u > 1.0)
        return false;

    float t = f * planeVector[1].dot(q);
    if (t > EPSILON && t > tmin) {
        if (record.t < 0.0f || record.t > t) {
            // ray.origin + t * ray.velocity is intersection point
            record.t = t;
            record.point = ray.pointAt(t);
            record.point = transformMat * record.point;
            if(m_point[0]->m_hasOther) {
                record.normal = m_point[0]->m_normal + u * (m_point[1]->m_normal - m_point[0]->m_normal) + v * (m_point[2]->m_normal - m_point[0]->m_normal);
            } else {
                record.normal = planeVector[0].cross(planeVector[1]).normalize();
            }
            auto worldToObjectMat = glm::inverse(transformMat);
            auto noShearTransformMat = glm::mat3(glm::transpose(worldToObjectMat));
            record.normal = noShearTransformMat * record.normal;
            record.material = m_material;
            return true;
        }
    }
    return false;
}

void Triangle::createVAO(std::vector<ShadeObject *> &shadeObjectList, const glm::mat4 &transformMat) {
    // VAO
    uint32_t vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Mesh VBO
    uint32_t meshVbo;
    glGenBuffers(1, &meshVbo);
    glBindBuffer(GL_ARRAY_BUFFER, meshVbo);
    Vec3f pointList[3] = {m_point[0]->m_coord, m_point[1]->m_coord, m_point[2]->m_coord};
    glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(Vec3f), pointList, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    // Normal VBO
    Vec3f planeVector[2] = {m_point[1]->m_coord - m_point[0]->m_coord, m_point[2]->m_coord - m_point[0]->m_coord};
    Vec3f normal = planeVector[0].cross(planeVector[1]);
    Vec3f normalList[3] = {normal, normal, normal};
    uint32_t normalVbo;
    glGenBuffers(1, &normalVbo);
    glBindBuffer(GL_ARRAY_BUFFER, normalVbo);
    glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(Vec3f), normalList, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(1);

    // EBO
    uint32_t indices[3] = {0, 1, 2};
    uint32_t ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindVertexArray(0);

    shadeObjectList.push_back(new ShadeObject({vao, 3}, m_material, transformMat));

}

bool Triangle::readObjectInfo(const YAML::Node &node, const Scene *scene) {
    bool result = BLASNode::readObjectInfo(node, scene);
    if(!result) {
        return false;
    }

    for(int i = 0;i < 3;++i) {
        TriangleNode *triangleNode = new TriangleNode(Vec3f::toVec3f((*node.begin())["position"].as<std::vector<float>>()), {0, 0}, {0, 0, 0}, false);
        m_boundingBox.updateBoundingBox(triangleNode->m_coord);
        m_point[i] = triangleNode;
    }
    return result;
}

ObjectBoundingBox Triangle::getBoundingBox() const {
    return m_boundingBox;
}