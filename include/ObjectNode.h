//
// Created by Edge on 2020/7/7.
//

#ifndef RAY_OBJECTNODE_H
#define RAY_OBJECTNODE_H

#include <glm/ext/matrix_float4x4.hpp>
#include "Ray.h"
#include "Material.h"

struct ObjectInfo {
    uint32_t m_vao;
    int m_indicesAmount;
};

struct ShadeObject {
    ShadeObject(ObjectInfo objectInfo, Material *material, const glm::mat4& transformMat): m_objectInfo(objectInfo), m_material(material), m_transformMat(transformMat) {}
    ObjectInfo m_objectInfo;
    Material *m_material;
    glm::mat4 m_transformMat;
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

class ObjectNode {
public:
    ObjectNode() {}

    enum class TwoLevelBVHType {
        TLAS,
        BLAS
    };

    virtual bool isHit(const Ray &ray, IntersectionRecord &record, float tmin=0.0001f, const glm::mat4 &transformMat = glm::mat4(1.0)) const = 0;

    // For local shading
    virtual void createVAO(std::vector<ShadeObject *> &shadeObjectList, const glm::mat4 &transformMat = glm::mat4(1.0f)) = 0;

    virtual bool readObjectInfo(const YAML::Node &node, const Scene *scene) = 0;

    virtual TwoLevelBVHType getTypeInTwoLevelBVH() = 0;

    virtual ObjectBoundingBox getBoundingBox() const = 0;

    int m_inTwoLevelBVHId;
};

#endif //RAY_OBJECTNODE_H
