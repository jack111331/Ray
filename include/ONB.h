//
// Created by Edge on 2021/1/23.
//

#ifndef RAY_ONB_H
#define RAY_ONB_H


#include "Utility.h"

class ONB {
public:
    ONB (const Vec3f &direction);
    Vec3f toWorldCoordSystem(const Vec3f &localVector);

    Vec3f m_direction, m_tangent, m_cotangent;

};


#endif //RAY_ONB_H
