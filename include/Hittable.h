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

class Hittable {
public:
    virtual bool isHit(double tmin, const Ray &ray, HitRecord &record) = 0;

    // For local shading
    virtual std::vector<ObjectInfo> createVAO(const Material *material) {
        std::vector<ObjectInfo> result;
        result.push_back({0, 0});
        return result;
    };

    void setMaterial(Material *material) {
        m_material = material;
    }

    Material *m_material;

};

#endif //RAY_HITTABLE_H
