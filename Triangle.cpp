//
// Created by Edge on 2020/7/9.
//

#include "Triangle.h"

#include <cmath>
#include <GL/glew.h>

using namespace std;

bool Triangle::isHit(double tmin, const Ray &ray, HitRecord &record) {
    const double EPSILON = 1e-7;
    Velocity planeVector[2] = {m_point[1]->m_coord - m_point[0]->m_coord, m_point[2]->m_coord - m_point[0]->m_coord};
    Velocity h = ray.velocity.cross(planeVector[1]);
    double a = planeVector[0].dot(h);
    if (std::abs(a) < EPSILON)
        return false;
    double f = 1.0 / a;
    Velocity s = ray.origin - m_point[0]->m_coord;
    double u = f * s.dot(h);
    if (u < 0.0 || u > 1.0)
        return false;
    Velocity q = s.cross(planeVector[0]);
    double v = f * ray.velocity.dot(q);
    if (v < 0.0 || v + u > 1.0)
        return false;

    double t = f * planeVector[1].dot(q);
    if (t > EPSILON && t > tmin) {
        if (record.t < 0.0f || record.t > t) {
            // ray.origin + t * ray.velocity is intersection point
            record.t = t;
            record.point = ray.pointAt(t);
            record.normal = planeVector[0].cross(planeVector[1]).normalize();
            record.material = m_material;
            return true;
        }
    }
    return false;
}

vector<ObjectInfo> Triangle::createVAO(const Material *material) {
    // VAO
    uint32_t vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Mesh VBO
    uint32_t meshVbo;
    glGenBuffers(1, &meshVbo);
    glBindBuffer(GL_ARRAY_BUFFER, meshVbo);
    glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(Coord), m_point, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    // Normal VBO
    Velocity planeVector[2] = {m_point[1]->m_coord - m_point[0]->m_coord, m_point[2]->m_coord - m_point[0]->m_coord};
    Velocity normal = planeVector[0].cross(planeVector[1]);
    Velocity normalList[3] = {normal, normal, normal};
    uint32_t normalVbo;
    glGenBuffers(1, &normalVbo);
    glBindBuffer(GL_ARRAY_BUFFER, normalVbo);
    glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(Velocity), normalList, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(1);

    // EBO
    uint32_t indices[3] = {0, 1, 2};
    uint32_t ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindVertexArray(0);

    vector<ObjectInfo> result;
    result.push_back({vao, 3});

    return result;
}

bool Triangle::readObjectInfo(const YAML::Node &node, const Scene *scene) {
    bool result = Hittable::readObjectInfo(node, scene);
    if(!result) {
        return false;
    }

    for(int i = 0;i < 3;++i) {
        TriangleNode *triangleNode = new TriangleNode{Coord::toCoord((*node.begin())["position"].as<std::vector<float>>()), {0, 0}, {0, 0, 0}};
        m_boundingBox.updateBoundingBox(triangleNode->m_coord);
        m_point[i] = triangleNode;
    }
    return result;
}

ObjectBoundingBox Triangle::getBoundingBox() {
    return m_boundingBox;
}