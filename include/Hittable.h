//
// Created by Edge on 2020/7/7.
//

#ifndef RAY_HITTABLE_H
#define RAY_HITTABLE_H

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

    ObjectBoundingBox(const Coord &minCoord, const Coord &maxCoord) : m_bounding{minCoord, maxCoord} {}

    void updateBoundingBox(const Coord &coord) {
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

    bool isHit(const Ray &ray);

    Coord m_bounding[2];
};

class Hittable {
public:
    Hittable() : m_material(nullptr) {}

    virtual bool isHit(const Ray &ray, HitRecord &record, float tmin=0.0001f) const = 0;

    // For local shading
    virtual std::vector<ShadeObject *> createVAO() {
        std::vector<ShadeObject *> result;
        result.push_back(new ShadeObject({0, 0}, nullptr));
        return result;
    };

    void setMaterial(Material *material) {
        m_material = material;
    }

    virtual bool readObjectInfo(const YAML::Node &node, const Scene *scene);

    virtual ObjectBoundingBox getBoundingBox() const = 0;

    Material *m_material;

};

#endif //RAY_HITTABLE_H
