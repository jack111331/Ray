//
// Created by Edge on 2020/7/7.
//

#ifndef RAY_SPHERE_H
#define RAY_SPHERE_H

#include "Utility.h"
#include "Ray.h"
#include "Hittable.h"

class Sphere : public Hittable {
public:
    Sphere() : m_origin{0, 0, 0}, m_radius(0) {};

    Sphere(Coord origin, float radius) : m_origin(origin), m_radius(radius) {};

    virtual bool isHit(double tmin, const Ray &ray, HitRecord &record) const;

    virtual std::vector<ShadeObject *> createVAO();

    virtual bool readObjectInfo(const YAML::Node &node, const Scene *scene);

    virtual ObjectBoundingBox getBoundingBox() const;

    Coord m_origin;
    float m_radius;
};


#endif //RAY_SPHERE_H
