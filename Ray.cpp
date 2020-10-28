//
// Created by Edge on 2020/7/15.
//

#include "Ray.h"

Coord Ray::pointAt(double t) const {
    return origin + t * velocity;
}
