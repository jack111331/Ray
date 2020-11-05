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
    Sphere(Coord origin, double radius) : m_origin(origin), m_radius(radius) {};

    virtual bool isHit(double tmin, const Ray &ray, HitRecord &record);

    virtual std::vector<ObjectInfo> createVAO(const Material *material);

    Coord m_origin;
    double m_radius;
};


#endif //RAY_SPHERE_H
