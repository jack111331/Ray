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

    Sphere(Vec3f origin, float radius) : m_origin(origin), m_radius(radius) {};

    virtual bool isHit(const Ray &ray, IntersectionRecord &record, float tmin=0.0001f, const glm::mat4 &transformMat = glm::mat4(1.0)) const;

    virtual  void createVAO(std::vector<ShadeObject *> &shadeObjectList);

    virtual bool readObjectInfo(const YAML::Node &node, const Scene *scene);

    virtual ObjectBoundingBox getBoundingBox() const;

    Vec3f m_origin;
    float m_radius;
};


#endif //RAY_SPHERE_H
