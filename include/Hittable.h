//
// Created by Edge on 2020/7/7.
//

#ifndef RAY_HITTABLE_H
#define RAY_HITTABLE_H

#include <glm/ext/matrix_float4x4.hpp>
#include "Ray.h"
#include "Material.h"

struct ObjectInfo {
    uint32_t m_vao;
    int m_indicesAmount;
};

struct ShadeObject {
    ShadeObject(ObjectInfo objectInfo, Material *material): m_objectInfo(objectInfo), m_material(material) {}
    ObjectInfo m_objectInfo;
    Material *m_material;
};

class ObjectBoundingBox {
public:
    ObjectBoundingBox() : m_bounding{{1e9,  1e9,  1e9},
                                     {-1e9, -1e9, -1e9}} {}

    ObjectBoundingBox(const Vec3f &minCoord, const Vec3f &maxCoord) : m_bounding{minCoord, maxCoord} {}

    void updateBoundingBox(const Vec3f &coord) {
        for (int i = 0; i < 3; ++i) {
            m_bounding[0][i] = std::min(m_bounding[0][i], coord[i]);
            m_bounding[1][i] = std::max(m_bounding[1][i], coord[i]);
        }
    }

    void updateBoundingBox(const ObjectBoundingBox &boundingBox) {
        for (int i = 0; i < 3; ++i) {
            m_bounding[0][i] = std::min(m_bounding[0][i], boundingBox.m_bounding[0][i]);
            m_bounding[1][i] = std::max(m_bounding[1][i], boundingBox.m_bounding[1][i]);
        }
    }

    static ObjectBoundingBox transformToLocal(const glm::mat4 &toLocalMat, const ObjectBoundingBox &boundingBox) {
        glm::vec4 minBoundingBox(boundingBox.m_bounding[0].x, boundingBox.m_bounding[0].y, boundingBox.m_bounding[0].z, 1.0);
        glm::vec4 maxBoundingBox(boundingBox.m_bounding[1].x, boundingBox.m_bounding[1].y, boundingBox.m_bounding[1].z, 1.0);
        minBoundingBox = toLocalMat * minBoundingBox;
        maxBoundingBox = toLocalMat * maxBoundingBox;
        return {
                {std::min(minBoundingBox.x, maxBoundingBox.x), std::min(minBoundingBox.y, maxBoundingBox.y), std::min(minBoundingBox.z, maxBoundingBox.z)},
                {std::max(minBoundingBox.x, maxBoundingBox.x), std::max(minBoundingBox.y, maxBoundingBox.y), std::max(minBoundingBox.z, maxBoundingBox.z)}
        };
    }

    bool isHit(const Ray &ray, float &tNear, float &tFar);

    Vec3f m_bounding[2];
};

class Hittable {
public:
    Hittable() : m_material(nullptr) {}

    virtual bool isHit(const Ray &ray, IntersectionRecord &record, float tmin=0.0001f, const glm::mat4 &transformMat = glm::mat4(1.0)) const {
        std::cerr << "Not defined isHit" << std::endl;
        exit(1);
    }

    // For local shading
    virtual void createVAO(std::vector<ShadeObject *> &shadeObjectList) {
        std::cerr << "Not defined createVAO" << std::endl;
        exit(1);
    };

    void setMaterial(Material *material) {
        m_material = material;
    }

    virtual bool readObjectInfo(const YAML::Node &node, const Scene *scene);

    virtual ObjectBoundingBox getBoundingBox() const {
        std::cerr << "Not defined getBoundingBox" << std::endl;
        exit(1);
    }

    Material *m_material;

};

#endif //RAY_HITTABLE_H
