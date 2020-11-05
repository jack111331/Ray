//
// Created by Edge on 2020/7/7.
//

#ifndef RAY_RAY_H
#define RAY_RAY_H

#include "Utility.h"

class Ray {
public:
    Coord pointAt(double t) const;
    Coord origin;
    Velocity velocity;

    friend std::ostream& operator << (std::ostream& os, const Ray &ray) {
        os << "Origin=" << ray.origin << ", Velocity= " << ray.velocity;
    }
};

#endif //RAY_RAY_H
