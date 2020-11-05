//
// Created by Edge on 2020/7/13.
//

#ifndef RAY_LIGHT_H
#define RAY_LIGHT_H


#include "Utility.h"

class Light {
public:
    Light(Coord origin) : m_origin(origin) {}

    Coord m_origin;
};

#endif //RAY_LIGHT_H
