//
// Created by Edge on 2020/12/25.
//

#ifndef RAY_TRIANGLEGROUP_H
#define RAY_TRIANGLEGROUP_H

#include "Hittable.h"
#include "Triangle.h"
#include "BVH.h"

class TriangleGroup;


namespace objl {
    class Mesh;
}

// TODO template specialization
class BVH {
public:
    BVH(TriangleGroup *triangleGroup);

    bool isHit(double tmin, const Ray &ray, HitRecord &record);

    void updateBVH(TriangleGroup *triangleGroup);

    Octree<Triangle> *m_octree;
};

class TriangleGroup : public Hittable {
public:
    TriangleGroup() : Hittable(), m_accel(nullptr), m_individualTriangle(false) {}

    static std::vector<Hittable *> fromObj(const YAML::Node &node, const Scene *scene);

    bool fromObj(const objl::Mesh &mesh, const Scene *scene);

    bool readObjectInfo(const YAML::Node &node, const Scene *scene);

    virtual bool isHit(double tmin, const Ray &ray, HitRecord &record) const;

    // For local shading
    virtual std::vector<ShadeObject *> createVAO();

    virtual ObjectBoundingBox getBoundingBox() const;

public:
    std::vector<Triangle *> m_triangles;
    ObjectBoundingBox m_boundingBox;

    bool m_individualTriangle;

    BVH *m_accel;
};


#endif //RAY_TRIANGLEGROUP_H
