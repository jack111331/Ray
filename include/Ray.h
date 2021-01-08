//
// Created by Edge on 2020/7/7.
//

#ifndef RAY_RAY_H
#define RAY_RAY_H

#include "Utility.h"

class Ray {
public:
    Vec3f pointAt(double t) const;
    Vec3f origin;
    Vec3f velocity;

    friend std::ostream& operator << (std::ostream& os, const Ray &ray) {
        os << "Origin=" << ray.origin << ", Velocity= " << ray.velocity;
    }
};

#endif //RAY_RAY_H
