//
// Created by Edge on 2020/7/7.
//

#ifndef ADVANCED_COMPUTER_GRAPH_SPHERE_H
#define ADVANCED_COMPUTER_GRAPH_SPHERE_H

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


#endif //ADVANCED_COMPUTER_GRAPH_SPHERE_H
